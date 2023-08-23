/**
 * 5 semestre - Eng. da Computação - Insper
 * Rafael Corsi - rafael.corsi@insper.edu.br
 *
 * Projeto 0 para a placa SAME70-XPLD
 *
 * Objetivo :
 *  - Introduzir ASF e HAL
 *  - Configuracao de clock
 *  - Configuracao pino In/Out
 *
 * Material :
 *  - Kit: ATMEL SAME70-XPLD - ARM CORTEX M7
 */

/************************************************************************/
/* includes                                                             */
/************************************************************************/

#include "asf.h"

/************************************************************************/
/* defines                                                              */
/************************************************************************/

#define LED_PIO           PIOC                 // periferico que controla o LED// # (1)
#define LED_PIO_ID        ID_PIOC                  // ID do periférico PIOC (controla LED)
#define LED_PIO_IDX       8                    // ID do LED no PIO
#define LED_PIO_IDX_MASK  (1 << LED_PIO_IDX)   // Mascara para CONTROLARMOS o LED

// Configuracoes do botao
#define BUT_PIO			 PIOA
#define BUT_PIO_ID		 ID_PIOA
#define BUT_PIO_IDX      11
#define BUT_PIO_IDX_MASK (1u << BUT_PIO_IDX) // esse já está pronto.

#define OLED_LED1		 PIOA	
#define OLED_LED1_ID	 ID_PIOA
#define OLED_LED1_IDX    0
#define OLED_LED1_IDX_MASK (1u << OLED_LED1_IDX)


#define OLED_LED2		 PIOC
#define OLED_LED2_ID	 ID_PIOC
#define OLED_LED2_IDX    30
#define OLED_LED2_IDX_MASK (1u << OLED_LED2_IDX)


#define OLED_LED3		 PIOB
#define OLED_LED3_ID	 ID_PIOB
#define OLED_LED3_IDX    2
#define OLED_LED3_IDX_MASK (1u << OLED_LED3_IDX)

#define OLED_BUT1		PIOD
#define OLED_BUT1_ID	ID_PIOD
#define OLED_BUT1_IDX	28
#define OLED_BUT1_IDX_MASK	(1u << OLED_BUT1_IDX)

#define OLED_BUT2		PIOC
#define OLED_BUT2_ID	ID_PIOC
#define OLED_BUT2_IDX	31
#define OLED_BUT2_IDX_MASK	(1u << OLED_BUT2_IDX)

#define OLED_BUT3		PIOA
#define OLED_BUT3_ID	ID_PIOA
#define OLED_BUT3_IDX	19
#define OLED_BUT3_IDX_MASK	(1u << OLED_BUT3_IDX)

/************************************************************************/
/* constants                                                            */
/************************************************************************/

/************************************************************************/
/* variaveis globais                                                    */
/************************************************************************/

/************************************************************************/
/* prototypes                                                           */
/************************************************************************/

void pio_set_output (    Pio *  p_pio,
const uint32_t  ul_mask,
const uint32_t  ul_default_level,
const uint32_t  ul_multidrive_enable,
const uint32_t  ul_pull_up_enable
);

void init(void);

/************************************************************************/
/* interrupcoes                                                         */
/************************************************************************/

/************************************************************************/
/* funcoes                                                              */
/************************************************************************/

// Função de inicialização do uC
void init(void)
{
	// Initialize the board clock
	sysclk_init();

	// Desativa WatchDog Timer
	WDT->WDT_MR = WDT_MR_WDDIS;
	
	// Ativa o PIO na qual o LED foi conectado
	// para que possamos controlar o LED.
	pmc_enable_periph_clk(LED_PIO_ID);
	
	//Inicializa PC8 como saída
	pio_set_output(LED_PIO, LED_PIO_IDX_MASK, 0, 0, 0);
	
	// Inicializa PIO do botao
	pmc_enable_periph_clk(BUT_PIO_ID);
	
	// configura pino ligado ao botão como entrada com um pull-up.
	pio_set_input(BUT_PIO, BUT_PIO_IDX_MASK, PIO_DEFAULT);
		
	pio_pull_up(BUT_PIO, BUT_PIO_IDX_MASK, 1);
	
	// Inicializa o OLED
	pmc_enable_periph_clk(OLED_LED1_ID);
	pmc_enable_periph_clk(OLED_LED2_ID);
	pmc_enable_periph_clk(OLED_LED3_ID);
	
	pio_set_output(OLED_LED1, OLED_LED1_IDX_MASK, 1, 0, 0);
	pio_set_output(OLED_LED2, OLED_LED2_IDX_MASK, 1, 0, 0);
	pio_set_output(OLED_LED3, OLED_LED3_IDX_MASK, 1, 0, 0);
	
	// Inicializa os botões do OLED
	pmc_enable_periph_clk(OLED_BUT1_ID);
	pmc_enable_periph_clk(OLED_BUT2_ID);
	pmc_enable_periph_clk(OLED_BUT3_ID);
	
	pio_set_input(OLED_BUT1, OLED_BUT1_IDX_MASK, PIO_DEFAULT);
	pio_set_input(OLED_BUT2, OLED_BUT2_IDX_MASK, PIO_DEFAULT);
	pio_set_input(OLED_BUT3, OLED_BUT3_IDX_MASK, PIO_DEFAULT);
	
	pio_pull_up(OLED_BUT1, OLED_BUT1_IDX_MASK, 1);
	pio_pull_up(OLED_BUT2, OLED_BUT2_IDX_MASK, 1);
	pio_pull_up(OLED_BUT3, OLED_BUT3_IDX_MASK, 1);
	
	
}

/************************************************************************/
/* Main                                                                 */
/************************************************************************/

// Funcao principal chamada na inicalizacao do uC.
int main(void)
{
  init();

  // super loop
  // aplicacoes embarcadas não devem sair do while(1).
  while (1)
  {
	    // Verifica valor do pino que o botão está conectado
	    if(!pio_get(OLED_BUT1, PIO_INPUT, OLED_BUT1_IDX_MASK)) {
		    // Pisca LED
		    for (int i=0; i<5; i++) {
			    pio_clear(OLED_LED1, OLED_LED1_IDX_MASK);  // Limpa o pino LED_PIO_PIN
			    delay_ms(100);                         // delay
			    pio_set(OLED_LED1, OLED_LED1_IDX_MASK);    // Ativa o pino LED_PIO_PIN
			    delay_ms(100);                         // delay
		    }
		    } else  {
		    // Ativa o pino LED_IDX (par apagar)
		    pio_set(OLED_LED1, OLED_LED1_IDX_MASK);
	    }	
		// Verifica valor do pino que o botão está conectado
		if(!pio_get(OLED_BUT2, PIO_INPUT, OLED_BUT2_IDX_MASK)) {
			// Pisca LED
			for (int i=0; i<5; i++) {
				pio_clear(OLED_LED2, OLED_LED2_IDX_MASK);  // Limpa o pino LED_PIO_PIN
				delay_ms(100);                         // delay
				pio_set(OLED_LED2, OLED_LED2_IDX_MASK);    // Ativa o pino LED_PIO_PIN
				delay_ms(100);                         // delay
			}
			} else  {
			// Ativa o pino LED_IDX (par apagar)
			pio_set(OLED_LED2, OLED_LED2_IDX_MASK);
		}
		// Verifica valor do pino que o botão está conectado
		if(!pio_get(OLED_BUT3, PIO_INPUT, OLED_BUT3_IDX_MASK)) {
			// Pisca LED
			for (int i=0; i<5; i++) {
				pio_clear(OLED_LED3, OLED_LED3_IDX_MASK);  // Limpa o pino LED_PIO_PIN
				delay_ms(100);                         // delay
				pio_set(OLED_LED3, OLED_LED3_IDX_MASK);    // Ativa o pino LED_PIO_PIN
				delay_ms(100);                         // delay
			}
			} else  {
			// Ativa o pino LED_IDX (par apagar)
			pio_set(OLED_LED1, OLED_LED1_IDX_MASK);
		}
  }
  return 0;
}
