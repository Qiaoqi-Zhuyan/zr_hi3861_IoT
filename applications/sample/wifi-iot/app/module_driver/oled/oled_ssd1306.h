/*
 * Copyright (c) 2021 Chinasoft International Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef OLED_SSD1306_H
#define OLED_SSD1306_H

// #include <stdint.h>

#include "ohos_types.h"

/**
 * @brief ssd1306 OLED Initialize.
 */
uint32 OledInit(void);

/**
 * @brief Set cursor position
 *
 * @param x the horizontal posistion of cursor
 * @param y the vertical position of cursor 
 * @return Returns {@link WIFI_IOT_SUCCESS} if the operation is successful;
 * returns an error code defined in {@link wifiiot_errno.h} otherwise.
 */
void OledSetPosition(uint8 x, uint8 y);

void OledFillScreen(uint8 fillData);

enum Font {
    FONT6x8 = 1,
    FONT8x16
};
typedef enum Font Font;

void OledShowChar(uint8 x, uint8 y, uint8 ch, Font font);
void OledShowString(uint8 x, uint8 y, const char* str, Font font);
void OLED_ShowNum(uint8 x, uint8 y, uint32 num, uint8 len, uint8 size);
#endif // OLED_SSD1306_H
