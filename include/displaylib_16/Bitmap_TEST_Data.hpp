/*!
	@file     ST7735_TFT_Bitmap_Data.hpp
	@author   Gavin Lyons
	@brief    Library header file for ST7735_TFT_PICO bitmap data
              Data file to hold bitmap data. Test 12 , test 14. 15 16	
    @note    Not part of core library only used for holding test data, should be omitted
    in project use or port.
*/

#pragma once
#include <array>
#include <span>
#include "pico/stdlib.h"

/**< Pointers to bitmap data which is in cpp file */
extern const std::span<const uint8_t> pSuperMarioWorldTitle;
extern const std::span<const uint8_t> pMarioIdleS1;
extern const std::span<const uint8_t> pMarioIdleS2;
extern const std::span<const uint8_t> pMarioWalkS1;
extern const std::span<const uint8_t> pMarioWalkS2;
extern const std::span<const uint8_t> pMarioJumpS1;
extern const std::span<const uint8_t> pMarioJumpS2;
extern const std::span<const uint8_t> pKoopaIdle1;
extern const std::span<const uint8_t> pDirt;
extern const std::span<const uint8_t> pGrass;
extern const std::span<const uint8_t> pLuckBlock1;
extern const std::span<const uint8_t> pRoBlock1;
extern const std::span<const uint8_t> pWoodBlock;
extern const std::span<const uint8_t> pPipeL;
extern const std::span<const uint8_t> pPipeTL;
extern const std::span<const uint8_t> pPipeR;
extern const std::span<const uint8_t> pPipeTR;
extern const std::span<const uint8_t> pSky;
