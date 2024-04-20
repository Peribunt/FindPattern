		FORCEINLINE
		VOID
		PreparePatternSearchMask( 
			OUT UINT64* ExpandedMask, 
			OUT UINT64* CorrectedBitmask,
			IN  UINT32  PatternLength
			)
		{
			*CorrectedBitmask = _byteswap_uint64( *CorrectedBitmask << ( 64 - PatternLength ) );
		
			for ( UINT32 i = NULL; i < sizeof( UINT64 ); i++ )
			{
				ExpandedMask[ i ] = 0xFF * ( ( ( ( ( ( __BYTE( *CorrectedBitmask, i ) ) * 0x0101010101010101 ) & 
					0x102040810204080 ) + 0x7F7E7C7870604000 ) >> 7 ) & 0x0101010101010101 );
			}
		}

DECLSPEC_NOINLINE
LPVOID
FindPattern( 	
	IN LPVOID      BaseAddress,
	IN UINT64      SearchLength,
	IN UINT8*      BytePattern,
	IN UINT32      PatternLength,
	IN UINT64      SearchMask
	)
{
	__m128i AlignedBytePattern[ 6 ]{ };
	__m128i ExpandedMasks     [ 6 ]{ };

	__m128i IdentifierValue = _mm_set1_epi8( BytePattern[ 0 ] );

	PreparePatternSearchMask( ( UINT64* )&ExpandedMasks[ 1 ], &SearchMask, PatternLength );

	AlignedBytePattern[ 1 ] = _mm_loadu_si128( ( __m128i* )BytePattern + 0 );
	AlignedBytePattern[ 2 ] = _mm_loadu_si128( ( __m128i* )BytePattern + 1 );
	AlignedBytePattern[ 3 ] = _mm_loadu_si128( ( __m128i* )BytePattern + 2 );
	AlignedBytePattern[ 4 ] = _mm_loadu_si128( ( __m128i* )BytePattern + 3 );

	UINT8*  SearchBase = ( UINT8* )ALIGN_LOW( BaseAddress, 0x10 );
	while ( SearchBase < ( UINT8* )BaseAddress + SearchLength - PatternLength )
	{
		__m128i IdentifierMask = _mm_cmpeq_epi8( _mm_loadu_si128( ( __m128i* )SearchBase ), IdentifierValue );

		if ( _mm_testz_si128( IdentifierMask, IdentifierMask ) == NULL )
		{
			UINT16 IdMask32 = _mm_movemask_epi8( IdentifierMask );

		CURRENT_SIG_RETRY:

			UINT32 TrailingZeros = _tzcnt_u32( IdMask32 );

			__m128i* BlockAlignedPattern = ( __m128i* )( ( UINT64 )&AlignedBytePattern[ 0 ] + ( 16 - TrailingZeros ) );
			__m128i* BlockAlignedMasks   = ( __m128i* )( ( UINT64 )&ExpandedMasks     [ 0 ] + ( 16 - TrailingZeros ) );
			
			for ( UINT32 i = NULL; i < 5; i++ )
			{
				__m128i Result = _mm_xor_si128( _mm_load_si128( ( __m128i* )SearchBase + i ), _mm_loadu_si128( BlockAlignedPattern + i ) );
				
				if ( _mm_testz_si128( Result, _mm_loadu_si128( BlockAlignedMasks + i ) ) == NULL )
				{
					if ( _mm_popcnt_u32( IdMask32 ) > 1 )
					{
						IdMask32 = IdMask32 & ~( 1 << TrailingZeros );
						goto CURRENT_SIG_RETRY;
					}

					goto LOOP_NEXT;
				}
			}

			return SearchBase + TrailingZeros;
		}

	LOOP_NEXT:
		SearchBase += 0x10;
	}

	return NULL;
}
