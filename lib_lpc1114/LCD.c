#include "LCD.h"

void LCD_pin_config(){

    // DIREÇÃO DOS PINOS
    LCD_RS_PORT->DIR |= (1 << LCD_RS_PIN);
    LCD_E_PORT->DIR  |= (1 << LCD_E_PIN);
    LCD_D4_PORT->DIR |= (1 << LCD_D4_PIN);
    LCD_D5_PORT->DIR |= (1 << LCD_D5_PIN);
    LCD_D6_PORT->DIR |= (1 << LCD_D6_PIN);
    LCD_D7_PORT->DIR |= (1 << LCD_D7_PIN);

    // CONFIGURAÇÃO IOCON (sem função alternativa)
    LPC_IOCON->PIO3_1  = 0x00; // RS
    LPC_IOCON->PIO1_4  = 0x80; // E
    LPC_IOCON->PIO1_11 = 0x80; // D4
    LPC_IOCON->PIO3_2  = 0x00; // D5
    LPC_IOCON->PIO1_5  = 0x00; // D6
    LPC_IOCON->PIO3_3  = 0x00; // D7

    LPC_GPIO2->DIR |= (1 << 3);        // RW como saída
    LPC_IOCON->PIO2_3 = 0x00;          // função GPIO padrão
    LPC_GPIO2->DATA &= ~(1 << 3);      // força RW = 0 (escrita)
}

// === FUNÇÕES AUXILIARES ===
void bit_set(volatile uint32_t *end, uint8_t bit) {
    (*end) |= (1 << bit);
}

void bit_clr(volatile uint32_t *end, uint8_t bit) {
    (*end) &= ~(1 << bit);
}

void delay_ms(unsigned int tempo) {
    for (unsigned int i = 0; i < tempo * 1000; i++) __NOP(); // Aproximadamente 1 ms com 12 MHz
}


// === ENVIA 4 BITS PARA O LCD ===
void envia_nibble(uint8_t nibble) {
    (nibble & 0x01) ? bit_set(&LCD_D4_PORT->DATA, LCD_D4_PIN) : bit_clr(&LCD_D4_PORT->DATA, LCD_D4_PIN);
    (nibble & 0x02) ? bit_set(&LCD_D5_PORT->DATA, LCD_D5_PIN) : bit_clr(&LCD_D5_PORT->DATA, LCD_D5_PIN);
    (nibble & 0x04) ? bit_set(&LCD_D6_PORT->DATA, LCD_D6_PIN) : bit_clr(&LCD_D6_PORT->DATA, LCD_D6_PIN);
    (nibble & 0x08) ? bit_set(&LCD_D7_PORT->DATA, LCD_D7_PIN) : bit_clr(&LCD_D7_PORT->DATA, LCD_D7_PIN);
}

// === ENVIA UM COMANDO OU CARACTERE COMPLETO (8 BITS) ===
void LCD_envia_byte(uint8_t dado, int comando) {
    if (comando)
        bit_clr(&LCD_RS_PORT->DATA, LCD_RS_PIN);  // comando
    else
        bit_set(&LCD_RS_PORT->DATA, LCD_RS_PIN);  // caractere

    envia_nibble(dado >> 4); // parte alta
    pulso_enable();

    envia_nibble(dado & 0x0F); // parte baixa
    pulso_enable();

  
}

// === ENVIA UM COMANDO DIRETAMENTE ===
void LCD_cmd(uint8_t cmd) {
    LCD_envia_byte(cmd, 1);
      delay_ms(2);
}

// === ENVIA UM CARACTERE DIRETAMENTE ===
void LCD_caractere(const char *caractere){
	LCD_envia_byte(*caractere, 0);

}

// === ESCREVE UMA STRING NO DISPLAY ===
void LCD_escreve(const char *texto) {
	int cont = 0;
    while (*texto) {
        if(cont == 16) LCD_cmd(0xC0);
        else if(cont == 32) break;
    	LCD_envia_byte(*texto++, 0);
        cont += 1;

    }
}

// === CONFIGURAÇÃO INICIAL DO LCD ===
void LCD_iniciar()
{
    bit_clr(&LCD_E_PORT->DATA,  LCD_E_PIN);
    bit_clr(&LCD_RS_PORT->DATA, LCD_RS_PIN);

    delay_ms(100);

    envia_nibble(0x03); pulso_enable(); delay_ms(5);
    envia_nibble(0x03); pulso_enable(); delay_ms(5);
    envia_nibble(0x03); pulso_enable(); delay_ms(5);
    envia_nibble(0x02); pulso_enable(); delay_ms(5);  // Inicializa em 4 bits

    LCD_cmd(0x28); delay_ms(5); // 4 bits, 2 linhas, fonte 5x8
    LCD_cmd(0x0C); delay_ms(5); // Display ON, cursor OFF
    LCD_cmd(0x06); delay_ms(5); // incremento automático
    LCD_cmd(0x01); delay_ms(5); // limpa display
}

void pulso_enable() {
    bit_set(&LCD_E_PORT->DATA, LCD_E_PIN);
    //delay_ms(1);
    bit_clr(&LCD_E_PORT->DATA, LCD_E_PIN);
}

void LCD_set_cursor(uint8_t linha, uint8_t coluna)
{
    uint8_t endereco;

    // Corrigir coluna (máximo 15)
    if (coluna > 15) coluna = 15;

    switch (linha) {
        case 0:
            endereco = 0x00 + coluna;
            break;
        case 1:
            endereco = 0x40 + coluna;
            break;
        default:
            endereco = 0x00; // fallback para primeira posição
            break;
    }

    LCD_cmd(0x80 | endereco);
}

char* convertBCD_ASCII(char word){

  static char buffer[3];

  // Somo o zero  com aspas simples para conversão para ASCII
  buffer[0] = ((word >> 4) & 0x0F) + '0';  // dezena
  buffer[1] = (	word 	   & 0x0F) + '0';  // unidade
  buffer[2] = '\0';

  return buffer;

}

void LCD_Int(int num){

	char buffer[6];
    if (num < 0) {
        num = num * (-1);
        LCD_envia_byte('-', 1);
    }
    buffer[0] = (num / 10000) % 10 + 48;
    buffer[1] = (num / 1000) % 10 + 48;
    buffer[2] = (num / 100) % 10 + 48;
    buffer[3] = (num / 10) % 10 + 48;
    buffer[4] = (num / 1) % 10 + 48;
    buffer[5] = '\0';

    LCD_escreve(buffer);
}
