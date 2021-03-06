

// vars usadas para posicionamento de texto generico na tela
uint16_t gPosTxt_X = 0;
uint16_t gPosTxt_Y = 0;
uint16_t gTxtSize = 15;


// valores padrao de tamanho e altura dos botoes
int gTamBotao_W = 150;
int gTamBotao_H = 150;

// valores padrao de espacamento X e Y dos botoes. A altura é iniciada como -1 pois é definida posteriormente no codigo
int gOffset_W = 40;
int gOffset_H = -1;

// var que contem a posicao atual X para desenhar os botoes
int gPosicaoAtual_X = 0;

// total de botoes desenhados em uma determinada tela -- usado nos botoes de torneira
int gTotalBotoes = 0;

// vers que contem o x e y de um touch na tela
volatile uint16_t gTouch_X;
volatile uint16_t gTouch_Y;

	

//bool gBotao1_Apertado = false;
//bool gBotao2_Apertado = false;
//bool gBotao3_Apertado = false;


// var com a posicao Y padrao para desenhar botoes
#define POSICAO_PADRAO_BTN_Y 154




unsigned long Timeout_TelaAtiva_time_inicio;
unsigned long Timeout_TelaAtiva_time_atual;
unsigned long Timeout_TelaAtiva_time_tempo_passado;

int Timeout_TelaAtiva_SegundosPassados;
int Timeout_Operacao_Last_SegundosPassados;






//tft.getCursor(&currentX,&currentY);


void TELA_SetFontSize(int FontSize)
{
	#ifdef ctTELA_BIB_USADA_Adafruit_RA8875

		tft.textEnlarge(FontSize);
		
	#elif ctTELA_BIB_USADA_RA8875

		tft.setFontScale(FontSize);

	#endif	
}


bool TELA_touchDetect()
{
	#ifdef ctTELA_BIB_USADA_Adafruit_RA8875

		return tft.touched();
		
	#elif ctTELA_BIB_USADA_RA8875

		return tft.touchDetect();

	#endif	
}


bool TELA_touchReadPixel(uint16_t *x, uint16_t *y)
{
	#ifdef ctTELA_BIB_USADA_Adafruit_RA8875

		return tft.touchRead(x, y);
		
	#elif ctTELA_BIB_USADA_RA8875

		tft.touchReadPixel(x, y);
		return true;		

	#endif	
}




void TELA_LimpaTela()
{
	delay(100);
	tft.fillScreen(RA8875_BLACK);
	tft.fillScreen(RA8875_BLACK);
}




 
void TELA_Texto_Centralizado(String Texto, String Cor, int Tamanho, uint16_t PosY)
{

	if (String(ctTELA_HARDWARE) == String(F("ER-TFTM070-5")))
	{

		if (Cor == F(""))
		{
			Cor = F("AMARELO");
		}


		if (Cor == F("BRANCO"))
		{
			tft.setTextColor(RA8875_WHITE);
		}

		if (Cor == F("AMARELO"))
		{
			tft.setTextColor(RA8875_YELLOW);
		}

		if (Cor == F("VERMELHO"))
		{
			tft.setTextColor(RA8875_RED);
		}

		if (Cor == F("VERDE"))
		{
			tft.setTextColor(RA8875_GREEN);
		}

		if (Cor == F("AZUL"))
		{
			tft.setTextColor(RA8875_BLUE);
		}

		if (Cor == F("MAGENTA"))
		{
			tft.setTextColor(RA8875_MAGENTA);
		}

		if (Cor == F("CINZA"))
		{
			tft.setTextColor(CinzaClaro);
		}


		TELA_SetFontSize(Tamanho);

		uint16_t PosX = 0;
		int TamanhoLetra_W = 0;
		int Ajuste_W = 10;

		if (Tamanho == 0){TamanhoLetra_W = 0;}
		if (Tamanho == 1){TamanhoLetra_W = 16;}
		if (Tamanho == 2){TamanhoLetra_W = 24;}
		if (Tamanho == 3){TamanhoLetra_W = 31;}



		PosX = (800 / 2) - ((Texto.length() / 2) * TamanhoLetra_W) - Ajuste_W;

		if ((Texto.length() == 10) && (Tamanho == 3))
		{
			PosX = PosX + 16;
		}

		tft.setCursor(PosX, PosY);


		tft.print(Texto);


	}
}





// Escreve um texto na tela
void TELA_Texto(String Texto, String Cor)
{

	if (ctTELA_HARDWARE == F("TERMINAL"))
	{
		LogTerm(Texto); 
	}


	if (String(ctTELA_HARDWARE) == String(F("ER-TFTM070-5")))
	{


		if (Cor == F(""))
		{
			Cor = F("AMARELO");
		}


		if (Cor == F("BRANCO"))
		{
			tft.setTextColor(RA8875_WHITE);
		}

		if (Cor == F("AMARELO"))
		{
			tft.setTextColor(RA8875_YELLOW);
		}

		if (Cor == F("VERMELHO"))
		{
			tft.setTextColor(RA8875_RED);
		}

		if (Cor == F("VERDE"))
		{
			tft.setTextColor(RA8875_GREEN);
		}

		if (Cor == F("AZUL"))
		{
			tft.setTextColor(RA8875_BLUE);
		}

		if (Cor == F("MAGENTA"))
		{
			tft.setTextColor(RA8875_MAGENTA);
		}


		TELA_SetFontSize(0);

		

		tft.setCursor(gPosTxt_X, gPosTxt_Y);

		//use the classic print an println command
		tft.print(Texto);

		gPosTxt_X = 0;
		gPosTxt_Y = gPosTxt_Y + gTxtSize;

		//tft.setCursor(gPosTxt_X, gPosTxt_Y);
		// tft.setCursor(gPosTxt_X, gPosTxt_Y);888

		if (gPosTxt_Y > 465)
		{
			TELA_LimpaTela();
			gPosTxt_Y = 0;

			// o abaixo ocorre pois quando atingimos o limite de texto que cabe na tela, 
			// limpamos a tela. Quando limpamos, a tela de debug (que no caso estava apontando para o teclado numerico)
			// nao renderizava de novo, devido ao jeito como a tela de teclado numerico eh feita. 
			// por isto, definimos de novo o teclado como nao renderizado, para que ele seja renderizado logo em seguida
			// pela rotina de desenho da tela
			if (gModoOperacao == F("DEBUG"))
			{
				//gTelaRenderizada_TecNum = false;
				gTelaRenderizada_TecAlfa = false;

			}


		}

	}  


}



void TELA_LogTerm_XY()
{


	char TheTexto[100]; 


	/*
	if (ctTELA_HARDWARE == "ER-TFTM070-5")
	{

		

		Serial.print("X = ");
		Serial.print(gTouch_X); 

		Serial.print(" | Y = ");
		Serial.print(gTouch_Y); 


		Serial.print("  --  X > ");
		Serial.print(gOffset_W);
		Serial.print(" e X < ");
		Serial.print(gTamBotao_W + gOffset_W);

		Serial.print("    |    Y < ");
		Serial.print(gOffset_H);
		Serial.print(" e Y < ");
		Serial.println(gTamBotao_H + gOffset_H);

		//tft.changeMode(TEXT);
	}
	*/


	if (gModoOperacao == F("OPERACAO"))
	{

		/*
		// botao1:
		sprintf(TheTexto,"BOT1: X = %d | Y = %d   --   X > %d e X < %d  |  Y > %d e Y < %d  -- posY = %d", gTouch_X, gTouch_Y, gOffset_W, gTamBotao_W + gOffset_W, gOffset_H, gTamBotao_H + gOffset_H, gPosTxt_Y);  
		TELA_Texto(TheTexto, "");

		// botao2:
		sprintf(TheTexto,"BOT2: X = %d | Y = %d   --   X > %d e X < %d  |  Y > %d e Y < %d  -- posY = %d", gTouch_X, gTouch_Y, 2 * gOffset_W + gTamBotao_W, 2 * gOffset_W + 2 * gTamBotao_W , gOffset_H, gTamBotao_H + gOffset_H, gPosTxt_Y);  
		TELA_Texto(TheTexto, "");

		// botao3:
		sprintf(TheTexto,"BOT3: X = %d | Y = %d   --   X > %d e X < %d  |  Y > %d e Y < %d  -- posY = %d", gTouch_X, gTouch_Y, 3 * gOffset_W + 2 * gTamBotao_W, 3 * gOffset_W + 3 * gTamBotao_W , gOffset_H, gTamBotao_H + gOffset_H, gPosTxt_Y);  
		TELA_Texto(TheTexto, "");
		*/


		// sair:
	    int btnSair_PosAtual_X = 680;
		int btnSair_PosAtual_Y = 20;

		int btnSair_Size_W = 130;
		int btnSair_Size_H = 60;	

		sprintf(TheTexto,"SAIR: X = %d | Y = %d   --   X > %d e X < %d  |  Y > %d e Y < %d  -- gBounce_ContaClick = %d", gTouch_X, gTouch_Y, btnSair_PosAtual_X, btnSair_PosAtual_X + btnSair_Size_W, btnSair_PosAtual_Y, btnSair_PosAtual_Y + btnSair_Size_H, gBounce_ContaClick);  
		TELA_Texto(TheTexto, F(""));

		LogTerm(TheTexto);

		
	}
	else
	{
		// Apenas X Y:
		sprintf(TheTexto, "XY: X = %d | Y = %d", gTouch_X, gTouch_Y);  
		TELA_Texto(TheTexto, F(""));
		//TELA_Texto(gModoOperacao, "VERMELHO");

		LogTerm(TheTexto);
	}

	//tft.changeMode(GRAPHIC);
	

}




void TELA_Render_MsgBox(String Titulo, String Texto)
{

	int MaxChars = 40;


	int Size_Fundo_W;
	int Size_Fundo_H;

	int TamanhoTexto;
	int TotalLinhas;
	
	int ContaLinha;

	int Size_Char_W = 16;
	int Size_Char_H = 25;

	int Inicio_X = Size_Char_W;
	//int Inicio_Y = Size_Char_H;
	int Inicio_Y = 150;

	String Texto_Ajustado = F("");

	int BarraTitulo_H;

	BarraTitulo_H = 40;

	if (Titulo == F(""))
	{
		BarraTitulo_H = 0;
	}


	if (gTelaRenderizada_MSGBOX == false)
	{


		// ajusta texto para incorporar quebra de linha


		TamanhoTexto = Texto.length();

		if (TamanhoTexto <= MaxChars)
		{
			TotalLinhas = 1;
		}
		else
		{
			if (TamanhoTexto % MaxChars == 0)
			{
				TotalLinhas = floor(TamanhoTexto / MaxChars);
			}
			else
			{
				TotalLinhas = floor(TamanhoTexto / MaxChars) + 1;
			}

			
		}
		
		/*
		for (int x=0 ; x <= TamanhoTexto ; x++)
		{
			if (Texto.substring(x, x+1) == "^")
			{
				TotalLinhas++;				
			}

			//LogTerm(F("Char: " + Texto.substring(x, 1) + " / TotalLinhas = " + TotalLinhas);

		}		
		*/
		
		Size_Fundo_W = (TamanhoTexto * Size_Char_W) + 3;
		Size_Fundo_H = (TotalLinhas * Size_Char_H) + 2;


		if (Size_Fundo_W > ((MaxChars * Size_Char_W) + 3))
		{
			Size_Fundo_W = (MaxChars * Size_Char_W) + 3;
		}


		//int Fundo_X = Inicio_X - Size_Char_W;
		int Fundo_X = 380 - Size_Fundo_W / 2;
		int Fundo_Y = Inicio_Y - Size_Char_H;
		int Fundo_W = Size_Fundo_W + 2 * Size_Char_W;
		int Fundo_H = TotalLinhas * Size_Char_H + 2 * Size_Char_H;

		//sombra
		tft.fillRoundRect(Fundo_X + 8, Fundo_Y - BarraTitulo_H + 8, Fundo_W, Fundo_H + BarraTitulo_H, 0, CinzaShadow);
		



		//fundo/frame branco
		tft.fillRoundRect(Fundo_X, Fundo_Y, Fundo_W, Fundo_H, 0, RA8875_WHITE);

		
		//titulo
		if (Titulo != F(""))
		{
			tft.fillRoundRect(Fundo_X, Fundo_Y - BarraTitulo_H, Fundo_W, BarraTitulo_H, 0, RA8875_WHITE);

			tft.fillRoundRect(Fundo_X + 2, Fundo_Y - BarraTitulo_H + 2, Fundo_W - 4, BarraTitulo_H - 2, 0, CinzaFundoTitMsgBox);

			TELA_SetFontSize(1);

			tft.setTextColor(RA8875_BLACK);
			tft.setCursor (Fundo_X + 6, Fundo_Y - BarraTitulo_H + 4);
			tft.print(Titulo);			
		}




		
		//tft.fillRoundRect(Inicio_X - Size_Char_W + 2, Inicio_Y - Size_Char_H + 2, Size_Fundo_W + 2 * Size_Char_W - 4, Size_Fundo_H + 2 * Size_Char_H - 4, 0, RA8875_BLUE);

		for (ContaLinha = 1 ; ContaLinha <= TotalLinhas ; ContaLinha++)
		{

			int Linha_X = Fundo_X + 2;
			int Linha_Y = Fundo_Y + 2;
			int Linha_W = Size_Fundo_W + 2 * Size_Char_W - 4;
			int Linha_H = Size_Fundo_H + 2 * Size_Char_H - 6;

			tft.fillRoundRect(Linha_X, Linha_Y, Linha_W, Linha_H, 0, RA8875_BLUE);
	
		}

	    //tft.fillRoundRect(Inicio_X, Inicio_Y, Size_Fundo_W, Size_Fundo_H, 20, RA8875_WHITE);
		//tft.fillRoundRect(LabelNum_PosAtual_X + 2, LabelNum_PosAtual_Y + 2, Size_LabelNum_W - 4, Size_LabelNum_H, 0, RA8875_MAGENTA);
					
		TELA_SetFontSize(1); 
		tft.setTextColor(RA8875_WHITE);
		//tft.setTextColor(RA8875_GREEN);


		//int Texto_X = Inicio_X + 1;
		int Texto_X = (380 - Size_Fundo_W / 2) + 1 + Size_Char_W;
		int Texto_Y = Inicio_Y - 4;



		String LinhaToPrint;


		for (ContaLinha = 1 ; ContaLinha <= TotalLinhas ; ContaLinha++)
		{

			//Texto_Y += (ContaLinha - 1) * Size_Char_H;
			if (ContaLinha == 1)
			{
				Texto_Y = Inicio_Y - 4;
			}
			else
			{
				Texto_Y = (Inicio_Y - 4) + (ContaLinha - 1) * Size_Char_H;
			}

			tft.setCursor (Texto_X, Texto_Y);

			int TextoString_Inicio = (ContaLinha - 1) * MaxChars;
			int TextoString_Fim = (ContaLinha) * MaxChars;

			LinhaToPrint = Texto.substring(TextoString_Inicio, TextoString_Fim);

			// remove o espaco se for a primeira linha
			if (LinhaToPrint.substring(0, 1) == F(" "))
			{
				LinhaToPrint = LinhaToPrint.substring(1);
			}

			//LinhaToPrint.replace("^", "\r\n\n");

			tft.print(LinhaToPrint);
			//tft.print(Texto.substring(0, MaxChars));

			//Texto_Y = ((ContaLinha - 1) * Size_Char_H);
	
		}		

		/*

		LogTerm(F("TamanhoTexto = " + String(TamanhoTexto));
		LogTerm(F("MaxChars = " + String(MaxChars));
		LogTerm(F("TotalLinhas = " + String(TotalLinhas));
		LogTerm(F("Size_Char_W = " + String(Size_Char_W));
		LogTerm(F("Size_Char_H = " + String(Size_Char_H));
		LogTerm(F("Size_Fundo_W = " + String(Size_Fundo_W));
		LogTerm(F("Size_Fundo_H = " + String(Size_Fundo_H));
		LogTerm(F("==================");		
		*/

	}

	gTelaRenderizada_MSGBOX = true;
				


}



void TELA_Render_Label(String Texto, uint16_t CorTexto, uint16_t CorTextoBack, int Tamanho, uint16_t PosX, uint16_t PosY, String Align)
{

	if (String(ctTELA_HARDWARE) == String(F("ER-TFTM070-5")))
	{

		int TamanhoLetra_W = 0;
		if (Tamanho == 0){TamanhoLetra_W = 7.5;}
		if (Tamanho == 1){TamanhoLetra_W = 16;}
		if (Tamanho == 2){TamanhoLetra_W = 24;}
		if (Tamanho == 3){TamanhoLetra_W = 33;}


		if (CorTexto == 0)
		{
			CorTexto = CinzaLabels;
		}

		if (CorTextoBack == -1)
		{
			tft.setTextColor(CorTexto);
		}
		else
		{
			tft.setTextColor(CorTexto, CorTextoBack);
		}
		


		if (Align == F("RIGHT"))
		{
			PosX = 800 - (Texto.length() * TamanhoLetra_W) - 10;
		}


		TELA_SetFontSize(Tamanho);

		tft.setCursor(PosX, PosY);

		tft.print(Texto);

	}
}





//Estes includes tem de ficar neste local devido a ordem das chamadas das funcoes. Senao, da erro
#include "../Teclado/ChoppBot_Teclado_NUM.h" 
#include "../Teclado/ChoppBot_Teclado_ALFA.h" 

#include "../RFID/ChoppBot_RFID_Main.h" 



// Funcao que inicializa a tela sendo usada no momento
void TELA_IniciaTela()
{
	//LogTerm(ctTELA_HARDWARE);
	

	if (String(ctTELA_HARDWARE) == String(F("ER-TFTM070-5")))
	{

		pinMode(RA8875_INT, INPUT);
		digitalWrite(RA8875_INT, HIGH);


		tft.begin(RA8875_800x480);  

		//tft.fillScreen(RA8875_BLACK);//fill screen black
		//tft.changeMode(TEXT);
		//tft.setCursor(0,0);

			
	

		#ifdef ctTELA_BIB_USADA_Adafruit_RA8875


			
			tft.displayOn(true);
			tft.GPIOX(true);      // Enable TFT - display enable tied to GPIOX
			tft.PWM1config(true, RA8875_PWM_CLK_DIV1024); // PWM output for backlight
			tft.PWM1out(255);
			//tft.fillScreen(RA8875_BLACK);

			/* Switch to text mode */  
			tft.textMode();

			
		#elif ctTELA_BIB_USADA_RA8875

			tft.touchBegin(RA8875_INT);//enable Touch support!

		#endif		

		tft.touchEnable(true);
	}

}





void TELA_Render_BotaoTorneira(int IndexBotao, String Texto, String Texto2, String Texto3, String Texto4, String Cor, String TipoBotao)
{

	if (String(ctTELA_HARDWARE) == String(F("ER-TFTM070-5")))
	{



		switch (ctMAX_TORNEIRAS)
		{

			case 1:
				break;
			case 2:
				break;
			case 3:
				break;
			case 4:
				break;

			case 5:
				gTamBotao_W = 115;
				gTamBotao_H = 140;
				gOffset_W = 37;	
				break;

			case 6:
				gTamBotao_W = 86;
				gTamBotao_H = 140;
				gOffset_W = 50;	
				break;

			case 10:
				gTamBotao_W = 50;
				gTamBotao_H = 80;	
				gOffset_W = 25;
				break;
			
			default:

				gTamBotao_W = 80;
				gTamBotao_H = 80;	
				gOffset_W = 40;

				break;				
		}


		
		

		int OffSet_TextoBotao_W = gTamBotao_W / 2 - 11;
		int OffSet_TextoBotao_H = gTamBotao_H / 2 - 27;

		int OffSet_TextoTitulo_W = 0;
		int OffSet_TextoTitulo_H = 126;

		int OffSet_TextoTitulo2_H = 25;
		int OffSet_TextoTitulo3_H = 50;
		int OffSet_TextoTitulo4_H = 75;


		/*
		RA8875_BLACK            0x0000
		RA8875_BLUE             0x001F
		RA8875_RED              0xF800
		RA8875_GREEN            0x07E0
		RA8875_CYAN             0x07FF
		RA8875_MAGENTA          0xF81F
		RA8875_YELLOW           0xFFE0  
		RA8875_WHITE            0xFFFF
		*/



		gPosicaoAtual_X = gOffset_W + (IndexBotao * gTamBotao_W) + (IndexBotao * gOffset_W);

		gPosicaoAtual_X = gPosicaoAtual_X - gOffset_W - gTamBotao_W;

		// Desenha o quadrado do botao
		if (Cor == F("BRANCO"))
		{
			tft.fillRect(gPosicaoAtual_X, gOffset_H, gTamBotao_W, gTamBotao_H, RA8875_WHITE);
		}

		if (Cor == F("AZUL"))
		{
			tft.fillRect(gPosicaoAtual_X, gOffset_H, gTamBotao_W, gTamBotao_H, RA8875_BLUE);
		}

		if (Cor == F("MAGENTA"))
		{
			tft.fillRect(gPosicaoAtual_X, gOffset_H, gTamBotao_W, gTamBotao_H, RA8875_MAGENTA);
		}

		if (Cor == F("PRETO"))
		{
			tft.fillRect(gPosicaoAtual_X, gOffset_H, gTamBotao_W, gTamBotao_H, RA8875_BLACK);
		}



		// Render o titulo dentro do botao
		TELA_SetFontSize(2);


		if ((Cor == F("PRETO")) || (Cor == F("AZUL")) )
		{
			tft.setTextColor(RA8875_WHITE);
		}		
		else
		{
			tft.setTextColor(RA8875_BLACK);
		}
		
		tft.setCursor (gPosicaoAtual_X + OffSet_TextoBotao_W, gOffset_H + OffSet_TextoBotao_H);

		tft.print (IndexBotao);		




		// Render o titulo 
		TELA_SetFontSize(0);
		//tft.changeMode(TEXT);

		tft.setTextColor(RA8875_WHITE);


		// Render Texto
		if (Texto != F(""))
		{
			tft.setCursor (gPosicaoAtual_X + OffSet_TextoTitulo_W, gOffset_H + OffSet_TextoBotao_H + OffSet_TextoTitulo_H);    
			tft.print (Texto);
		}



		// Render Texto2
		if (Texto2 != F(""))
		{

			tft.setCursor (gPosicaoAtual_X + OffSet_TextoTitulo_W, gOffset_H + OffSet_TextoBotao_H + OffSet_TextoTitulo_H + OffSet_TextoTitulo2_H); 

			tft.print (Texto2);
			//tft.print (gPosicaoAtual_X);
			//tft.print (" a ");
			//tft.print (gPosicaoAtual_X + gTamBotao_W);

		}


		// Render Texto3
		if (Texto3 != F(""))
		{

			tft.setCursor (gPosicaoAtual_X + OffSet_TextoTitulo_W, gOffset_H + OffSet_TextoBotao_H + OffSet_TextoTitulo_H + OffSet_TextoTitulo3_H); 

			tft.print (Texto3);
			//tft.print (gPosicaoAtual_X);
			//tft.print (" a ");
			//tft.print (gPosicaoAtual_X + gTamBotao_W);

		}


		if (Texto4 != F(""))
		{
			tft.setCursor (gPosicaoAtual_X + OffSet_TextoTitulo_W, gOffset_H + OffSet_TextoBotao_H + OffSet_TextoTitulo_H + OffSet_TextoTitulo4_H); 
			tft.print (Texto4);
		}

		gTotalBotoes++;

	}

}



 





void TELA_Render_BotaoGenerico(int Index, String TAG, String Texto, int TamanhoTextoBotao, uint16_t CorTexto, uint16_t CorBotao, uint16_t PosX, uint16_t PosY, uint16_t W, uint16_t H )
{

	if (String(ctTELA_HARDWARE) == String(F("ER-TFTM070-5")))
	{


		int TamanhoLetra_W = 0;
		if (TamanhoTextoBotao == 0){TamanhoLetra_W = 8;}

		if (TamanhoTextoBotao == 1)
		{
			if (Texto.length() >= 9)
			{
				TamanhoLetra_W = 18;
			}
			else
			{
				TamanhoLetra_W = 16;
			}
			
		}

		if (TamanhoTextoBotao == 2){TamanhoLetra_W = 24;}
		if (TamanhoTextoBotao == 3){TamanhoLetra_W = 33;}

		int TamanhoLetra_H = 0;
		if (TamanhoTextoBotao == 0){TamanhoLetra_H = 8;}
		if (TamanhoTextoBotao == 1){TamanhoLetra_H = 18;}



		uint16_t Local_PosX = PosX + (W / 2) - ((Texto.length() / 2) * TamanhoLetra_W);
		uint16_t Local_PosY = PosY + (H / 2) - TamanhoLetra_H;



		tft.fillRoundRect(PosX, PosY, W, H, 8, CorBotao);	

		TELA_SetFontSize(TamanhoTextoBotao);

		if (CorTexto == 0)
		{
			CorTexto = White;
		}

		tft.setTextColor(CorTexto);


		// verifica se o texto tem 2 linhas. separador = |
		bool _TextoDuasLinhas = false;

		for (int _ContaCarac = 0 ; _ContaCarac <= Texto.length() - 1 ; _ContaCarac++)
		{
			//LogTerm(String(F("Carac = ")) + String(Texto.substring(_ContaCarac, _ContaCarac + 1)));

			if (String(Texto.substring(_ContaCarac, _ContaCarac + 1)) == String(F("|")))
			{
				_TextoDuasLinhas = true;
			}
		}

		//_TextoDuasLinhas = true;

		if (_TextoDuasLinhas == true)
		{

			String _Linha1 = getValue(Texto, '|', 0);
			String _Linha2 = getValue(Texto, '|', 1);

			

			Local_PosX = PosX + (W / 2) - ((_Linha1.length() / 2) * TamanhoLetra_W);

			if (_Linha1.length() == 4)
			{
				Local_PosX = Local_PosX + 2;
			}

			if (_Linha1.length() == 5)
			{
				Local_PosX = Local_PosX - 2;
			}

			if (_Linha1.length() == 6)
			{
				Local_PosX = Local_PosX + 5;
			}

			if (_Linha1.length() == 7)
			{
				Local_PosX = Local_PosX - 4;
			}


		    tft.setCursor (Local_PosX, Local_PosY - 16); 
		    tft.print (_Linha1);	




		    Local_PosX = PosX + (W / 2) - ((_Linha2.length() / 2) * TamanhoLetra_W);

			if (_Linha2.length() == 4)
			{
				Local_PosX = Local_PosX + 2;
			}

			if (_Linha2.length() == 5)
			{
				Local_PosX = Local_PosX - 2;
			}

			if (_Linha2.length() == 6)
			{
				Local_PosX = Local_PosX + 5;
			}

			if (_Linha2.length() == 7)
			{
				Local_PosX = Local_PosX - 4;
			}

		    tft.setCursor (Local_PosX, Local_PosY + 16); 
		    tft.print (_Linha2);	

		}
		else
		{
		    tft.setCursor (Local_PosX, Local_PosY); 
		    tft.print (Texto);	
		}


	    gaBotoesGenTela[Index - 1] = String(Index) 	+ String(F(";")) + 
	    							 TAG 			+ String(F(";")) + 
	    							 Texto 			+ String(F(";")) + 
	    							 String(PosX) 	+ String(F(";")) + 
	    							 String(PosY) 	+ String(F(";")) + 
	    							 String(W) 		+ String(F(";")) + 
	    							 String(H);



	}


}


void TELA_Zera_BotaoGenericoTela()
{
	for (int t = 0 ; t <= ctMAX_BOTOES_GEN_TELA - 1 ; t++)
	{
		gaBotoesGenTela[t] = F("");
	}
}


void TELA_PrintNoTerm_BotaoGenericoTela()
{
	int EncontrouDados = 0;

	for (int z = 0 ; z <= ctMAX_BOTOES_GEN_TELA - 1 ; z++)
	{
		if (gaBotoesGenTela[z] != F(""))
		{
			EncontrouDados = 1;
			LogTerm( String(F("gaBotoesGenTela[")) + String(z) + String(F("] = ")) + gaBotoesGenTela[z] );
		}
	}

	if (EncontrouDados == 0)
	{
		LogTerm(F("gaBotoesGenTela esta vazia"));
	}
}






void TELA_Render_Interface_STANDBY()
{
	
	//LogTerm(ctTELA_HARDWARE);


    unsigned long TempoInicioPrograma = 0;

    TempoInicioPrograma = millis();

    //LogTerm(String(F("TempoInicioPrograma = ")) + String(TempoInicioPrograma));

    //if (TempoInicioPrograma >= 10000)
    if (TempoInicioPrograma >= 2592000) // 30 dias
    {
        ResetArduino();
    }



	if (gTelaRenderizada_STANDBY == false)
	{

        //LogTerm(F("== [Modo Atual: STANDBY] ==");




		if (ctTELA_HARDWARE == F("TERMINAL"))
		{  
			LogTerm(String(F("ChoppBot ")) + String(SKETCH_VERSION));

			if (NOME_LOJA_LINHA2 != F(""))
			{
				LogTerm(String(F("Licenciado para: ")) + String(NOME_LOJA_LINHA1) + String(F(" ")) + String(NOME_LOJA_LINHA2));
			}
			else
			{
				LogTerm(String(F("Licenciado para: ")) + String(NOME_LOJA_LINHA1));
			}

			LogTerm(F("Tecle algo no console para iniciar..."));
		}



		if (String(ctTELA_HARDWARE) == String(F("ER-TFTM070-5")))
		{  



			TELA_LimpaTela();

			
			// versao do choppbot
			TELA_SetFontSize(0); 
			tft.setTextColor(CinzaLabels);
			tft.setCursor (650, 10);			
			tft.print (String(F("ChoppBot ")) + String(SKETCH_VERSION)); 




			// Titulo
			TELA_Texto_Centralizado(String(NOME_LOJA_LINHA1), F("AMARELO"), 3, 105);

			if (NOME_LOJA_LINHA2 != F(""))
			{
				TELA_Texto_Centralizado(String(NOME_LOJA_LINHA2), F("AMARELO"), 3, 175);
			}

			if (NOME_LOJA_LINHA3 != F(""))
			{

				if (String(NOME_LOJA_LINHA2) == String(F("")))
				{
					TELA_Texto_Centralizado(String(NOME_LOJA_LINHA3), F("CINZA"), 2, 210);
				}
				else
				{
					TELA_Texto_Centralizado(String(NOME_LOJA_LINHA3), F("CINZA"), 2, 260);
				}



				
			}


			// texto explicativo
			tft.setTextColor(RA8875_WHITE);

  
			//tft.setCursor (195, 310);
			//TELA_SetFontSize(1); 
			TELA_Texto_Centralizado(String(F("Toque na tela para iniciar")), F("BRANCO"), 1, 360);

			//tft.print (F("Toque na tela para iniciar"));   



		}

		gTelaRenderizada_STANDBY = true;

	}


}



void TELA_Render_Interface_LOGIN()
{

	if (gTelaRenderizada_LOGIN == false)
	{

	    if (ctTELA_HARDWARE == String(F("ER-TFTM070-5")))
	    {
	        TELA_LimpaTela();
	    }

		
		//LogTerm(F("== [Modo Atual: LOGIN] ==");

		if (ctTELA_HARDWARE == F("TERMINAL"))
		{  

			LogTerm(F("Ola! Seja bem vindo!"));
			LogTerm(F("Escolha o seu metodo de identificacao e digite o numero correspondente no console:"));
			LogTerm(F("1 - LEITOR BIOMETRICO"));
			LogTerm(F("2 - LEITOR DE CARTAO"));
			//LogTerm(F("3 - ABRE TECLADO NUMERICO"));
			LogTerm(F("3 - ABRE TECLADO ALFA"));

			LogTerm(F("4 - ADMIN"));


		}


		if (String(ctTELA_HARDWARE) == String(F("ER-TFTM070-5")))
		{  


			#ifdef ctTELA_BIB_USADA_Adafruit_RA8875


				tft.textTransparent(RA8875_YELLOW);
				tft.textSetCursor (175, 30);
				TELA_SetFontSize(2);

				//char Texto1[50];
				//String(F("Ola! Seja vem vindo!")).toCharArray(Texto1, 50) ;


				tft.textTransparent(RA8875_WHITE);
				tft.textSetCursor (100, 130);
				TELA_SetFontSize(1); 

				//tft.textWrite (String(F("Escolha o seu metodo de identificacao:")));   

				gOffset_H = POSICAO_PADRAO_BTN_Y + 95;

				TELA_Render_BotaoTorneira(1, F("LEITOR BIOMETRICO"), F(""), F(""), F(""), F("BRANCO"), F("GERAL") );
				TELA_Render_BotaoTorneira(2, F("LEITOR DE CARTAO"),  F(""), F(""), F(""), F("AZUL"),   F("GERAL") );
				//TELA_Render_BotaoTorneira(3, F("ABRE TECLADO NUMERICO"), F(""), F(""), F("MAGENTA"), F("GERAL") );
				TELA_Render_BotaoTorneira(3, F("ABRE TECLADO ALFA"), F(""), F(""), F(""), F("MAGENTA"), F("GERAL") );

				// Area para chamar admin
				//tft.fillRect(700, 0, 100, 60, RA8875_WHITE);


			#elif ctTELA_BIB_USADA_RA8875

				tft.setTextColor(RA8875_YELLOW);
				tft.setCursor (175, 30);
				TELA_SetFontSize(2);
				tft.print (F("Ola! Seja vem vindo!"));   

				tft.setTextColor(RA8875_WHITE);
				tft.setCursor (100, 130);
				TELA_SetFontSize(1); 
				tft.print (F("Escolha o seu metodo de identificacao:"));   

				gOffset_H = POSICAO_PADRAO_BTN_Y + 95;

				TELA_Render_BotaoTorneira(1, F("LEITOR BIOMETRICO"), F(""), F(""), F(""), F("BRANCO"), F("GERAL") );
				TELA_Render_BotaoTorneira(2, F("LEITOR DE CARTAO"), F(""), F(""), F(""), F("AZUL"), F("GERAL") );
				//TELA_Render_BotaoTorneira(3, F("ABRE TECLADO NUMERICO"), F(""), F(""), F("MAGENTA"), F("GERAL") );
				TELA_Render_BotaoTorneira(3, F("ABRE TECLADO ALFA"), F(""), F(""), F(""), F("MAGENTA"), F("GERAL") );

				// Area para chamar admin
				//tft.fillRect(700, 0, 100, 60, RA8875_WHITE);


			#endif






		}

		gTelaRenderizada_LOGIN = true;

	}

}



void TELA_Render_Interface_ADMIN()
{

	

	//LogTerm(String(F("gTelaRenderizada_ADMIN = ")) + String(gTelaRenderizada_ADMIN));
	//LogTerm(String(F("gTelaRenderizada_OPERACAO = ")) + String(gTelaRenderizada_OPERACAO));

	if (gTelaRenderizada_ADMIN == false)
	{
		// Inicia a contagem do tempo
		Timeout_TelaAtiva_time_inicio = millis();
	}
	else
	{
		if ((ctTIMEOUT_TELA_ADMIN / 1000) - Timeout_TelaAtiva_SegundosPassados <= 0)
		{
			Timeout_TelaAtiva_time_inicio = millis();

			gTelaRenderizada_ADMIN = false;

		}
	}


    Timeout_TelaAtiva_time_atual = millis();
    Timeout_TelaAtiva_time_tempo_passado = Timeout_TelaAtiva_time_atual - Timeout_TelaAtiva_time_inicio;

    Timeout_TelaAtiva_SegundosPassados = floor(Timeout_TelaAtiva_time_tempo_passado / 1000);


	// Renderiza os segundos
	if (String(ctTELA_HARDWARE) == String(F("ER-TFTM070-5")))
	{  		

	    if (Timeout_TelaAtiva_SegundosPassados != Timeout_Operacao_Last_SegundosPassados)
	    {	

				TELA_SetFontSize(0); 
				tft.setTextColor(CinzaLabels, RA8875_BLACK);
				tft.setCursor (640, 450);			
				tft.print (F("Tempo: ")); 

				tft.setCursor (700, 450);
				tft.print (String((ctTIMEOUT_TELA_ADMIN / 1000) - Timeout_TelaAtiva_SegundosPassados) + String(F(" segundos   "))); 


		} 
	}

	//LogTerm(String(F("(ctTIMEOUT_TELA_ADMIN / 1000) - SegundosPassados = ")) + String((ctTIMEOUT_TELA_ADMIN / 1000) - Timeout_TelaAtiva_SegundosPassados));



	// sai se o tempo limite ocorrer
	if ((ctTIMEOUT_TELA_ADMIN / 1000) - Timeout_TelaAtiva_SegundosPassados <= 0)
	{


		// zera as vars para cada tentativa de login
		// efetua logoff
		gSessao_Logado = false;
		gSessao_IDUser = -1;
		gSessao_Nome = F("");
		gSessao_Nivel = -1;
		gSessao_SaldoAtual = -1;

		gTelaRenderizada_ADMIN = false;	

		gModoOperacao = F("STANDBY");  
		gModoOperacao_SubTela = F("");						



		LogTerm(F("MAIN: Timeout de tela ADMIN"));

		delay(1000);
		
	}

	// ====================================================================





	if (gTelaRenderizada_ADMIN == false)
	{

		

		if (ctTELA_HARDWARE == F("TERMINAL"))
		{  


			LogTerm(F("Selecione a opcao de administracao desejada:"));
		
		    LogTerm(F("99 - Sair"));		
		}


		if (String(ctTELA_HARDWARE) == String(F("ER-TFTM070-5")))
		{  		

			TELA_Zera_BotaoGenericoTela();

			TELA_SetFontSize(1); 

			tft.setTextColor(CinzaLabels);
			tft.setCursor (10, 440);			
			tft.print (F("Nome: ")); 


			tft.setTextColor(RA8875_WHITE);
			tft.setCursor (100, 440);			
			tft.print (gSessao_Nome); 



			TELA_Texto_Centralizado(F("Escolha a sua opcao:"), F("AMARELO"), 2, 120);



		    TELA_Render_BotaoGenerico(1, F("SAIR"), F("SAIR"), 1, White, Red, 10, 15, 130, 60);

		    //////////////////////////////////////


			// Cabecalho
			TELA_Render_Label(F("Administracao"), Green, -1, 2, 0, 10, F("RIGHT"));


			

			//TELA_PrintNoTerm_BotaoGenericoTela();


			int _Espacamento = 35;
			int _Offset_X = 25;
			int _TamW = 160;
			int _TamH = 160;
			int _PosX = 0;
			int _PosY = 230;
			int _ContaBtn = 0;
			String _TxtBotao = F("");


			_Offset_X = _Offset_X - _Espacamento;	// como o primeiro botao ja inclue um espacamento, eu subtraio ele para ter o offset real

			for (_ContaBtn = 1 ; _ContaBtn <= 4 ; _ContaBtn++)
			{
				_PosX = (_Espacamento * _ContaBtn) + (_TamW * (_ContaBtn - 1));

				switch (_ContaBtn)
				{
					case 1:
						_TxtBotao = F("USUARIOS");
						break;
					case 2:
						_TxtBotao = F("CHOPPS");
						break;
					case 3:
						_TxtBotao = F("ENGATADOS");
						break;
					case 4:
						_TxtBotao = F("OUTROS");
						break;
				}


				TELA_Render_BotaoGenerico(_ContaBtn + 1, _TxtBotao, _TxtBotao, 1, White, Blue, _Offset_X + _PosX, _PosY, _TamW, _TamH);
			}

			
			//TELA_PrintNoTerm_BotaoGenericoTela();



		}

		gTelaRenderizada_ADMIN = true;

	}




}







void TELA_Render_Interface_ADMIN_USUARIOS()
{


	if (gTelaRenderizada_ADMIN_USUARIOS == false)
	{
		// Inicia a contagem do tempo
		Timeout_TelaAtiva_time_inicio = millis();
	}
	else
	{
		if ((ctTIMEOUT_TELA_ADMIN / 1000) - Timeout_TelaAtiva_SegundosPassados <= 0)
		{
			Timeout_TelaAtiva_time_inicio = millis();

			gTelaRenderizada_ADMIN_USUARIOS = false;

		}
	}


    Timeout_TelaAtiva_time_atual = millis();
    Timeout_TelaAtiva_time_tempo_passado = Timeout_TelaAtiva_time_atual - Timeout_TelaAtiva_time_inicio;

    Timeout_TelaAtiva_SegundosPassados = floor(Timeout_TelaAtiva_time_tempo_passado / 1000);


	// Renderiza os segundos
	if (String(ctTELA_HARDWARE) == String(F("ER-TFTM070-5")))
	{  		

	    if (Timeout_TelaAtiva_SegundosPassados != Timeout_Operacao_Last_SegundosPassados)
	    {	

				TELA_SetFontSize(0); 
				tft.setTextColor(CinzaLabels, RA8875_BLACK);
				tft.setCursor (640, 450);			
				tft.print (F("Tempo: ")); 

				tft.setCursor (700, 450);
				tft.print (String((ctTIMEOUT_TELA_ADMIN / 1000) - Timeout_TelaAtiva_SegundosPassados) + String(F(" segundos   "))); 


		} 
	}




	// sai se o tempo limite ocorrer
	if ((ctTIMEOUT_TELA_ADMIN / 1000) - Timeout_TelaAtiva_SegundosPassados <= 0)
	{


		// zera as vars para cada tentativa de login
		// efetua logoff
		gSessao_Logado = false;
		gSessao_IDUser = -1;
		gSessao_Nome = F("");
		gSessao_Nivel = -1;
		gSessao_SaldoAtual = -1;

		gTelaRenderizada_ADMIN = false;	

		gModoOperacao = F("STANDBY");  
		gModoOperacao_SubTela = F("");						



		LogTerm(F("MAIN: Timeout de tela ADMIN USUARIOS"));

		delay(1000);
		
	}



	if (gTelaRenderizada_ADMIN_USUARIOS == false)
	{

		if (ctTELA_HARDWARE == F("TERMINAL"))
		{  
			LogTerm(F("Selecione a opcao de administracao desejada:"));
		
		    LogTerm(F("99 - Sair"));		
		}


		if (String(ctTELA_HARDWARE) == String(F("ER-TFTM070-5")))
		{  		

			TELA_Zera_BotaoGenericoTela();

			TELA_SetFontSize(1); 

			// usuario logado
			tft.setTextColor(CinzaLabels);
			tft.setCursor (10, 440);			
			tft.print (F("Nome: ")); 


			tft.setTextColor(RA8875_WHITE);
			tft.setCursor (100, 440);			
			tft.print (gSessao_Nome); 



			// Cabecalho
			TELA_Render_Label(F("Administracao"), Green, -1, 2, 0, 10, F("RIGHT"));
			TELA_Render_Label(F("Usuarios"), CinzaClaro, -1, 1, 0, 60, F("RIGHT"));

			// botao voltar
		    TELA_Render_BotaoGenerico(1, F("VOLTAR"), F("VOLTAR"), 1, White, Red, 10, 15, 130, 60);

		    // texto geral
			TELA_Texto_Centralizado(F("Escolha a sua opcao:"), F("AMARELO"), 2, 120);



			int _Espacamento = 25;
			int _Offset_X = 15;
			int _TamW = 134;
			int _TamH = 140;
			int _PosX = 0;
			int _PosY = 230;
			int _ContaBtn = 0;
			String _TxtBotao = F("");
			String _TAG_Botao = F("");


			_Offset_X = _Offset_X - _Espacamento;	// como o primeiro botao ja inclue um espacamento, eu subtraio ele para ter o offset real

			for (_ContaBtn = 1 ; _ContaBtn <= 5 ; _ContaBtn++)
			{
				_PosX = (_Espacamento * _ContaBtn) + (_TamW * (_ContaBtn - 1));

				switch (_ContaBtn)
				{
					case 1:
						_TxtBotao = F("Novo|Cartao");
						_TAG_Botao = F("NOVO_CARTAO");
						break;
					case 2:
						_TxtBotao = F("Adic.|Valor");
						_TAG_Botao = F("ADD_VALOR");
						break;
					case 3:
						_TxtBotao = F(" Subtr.|Valor");
						_TAG_Botao = F("SUB_VALOR");
						break;
					case 4:
						_TxtBotao = F("Definir|Saldo");
						_TAG_Botao = F("SET_SALDO");
						break;
					case 5:
						_TxtBotao = F("Trocar|Cartao");
						_TAG_Botao = F("TROCA_CARTAO");
						break;
				}


				TELA_Render_BotaoGenerico(_ContaBtn + 1, _TAG_Botao, _TxtBotao, 1, White, Blue, _Offset_X + _PosX, _PosY, _TamW, _TamH);
			}

			
			//TELA_PrintNoTerm_BotaoGenericoTela();



		}

		gTelaRenderizada_ADMIN_USUARIOS = true;

	}




}





void TELA_Render_Interface_ADMIN_CHOPPS()
{

}

void TELA_Render_Interface_ADMIN_ENGATADOS()
{

}



void TELA_Render_Interface_ADMIN_NOVO_CARD()
{



	if (gTelaRenderizada_ADMIN_NOVO_CARD == false)
	{
		// Inicia a contagem do tempo
		Timeout_TelaAtiva_time_inicio = millis();
	}
	else
	{
		if ((ctTIMEOUT_TELA_ADMIN / 1000) - Timeout_TelaAtiva_SegundosPassados <= 0)
		{
			Timeout_TelaAtiva_time_inicio = millis();

			gTelaRenderizada_ADMIN_NOVO_CARD = false;

		}
	}


    Timeout_TelaAtiva_time_atual = millis();
    Timeout_TelaAtiva_time_tempo_passado = Timeout_TelaAtiva_time_atual - Timeout_TelaAtiva_time_inicio;

    Timeout_TelaAtiva_SegundosPassados = floor(Timeout_TelaAtiva_time_tempo_passado / 1000);


	// Renderiza os segundos
	if (String(ctTELA_HARDWARE) == String(F("ER-TFTM070-5")))
	{  		

	    if (Timeout_TelaAtiva_SegundosPassados != Timeout_Operacao_Last_SegundosPassados)
	    {	



				TELA_SetFontSize(0); 
				tft.setTextColor(CinzaLabels, RA8875_BLACK);
				tft.setCursor (640, 450);			
				tft.print (F("Tempo: ")); 

				tft.setCursor (700, 450);
				tft.print (String((ctTIMEOUT_TELA_ADMIN / 1000) - Timeout_TelaAtiva_SegundosPassados) + String(F(" segundos   "))); 


		} 
	}




	// sai se o tempo limite ocorrer
	if ((ctTIMEOUT_TELA_ADMIN / 1000) - Timeout_TelaAtiva_SegundosPassados <= 0)
	{


		// zera as vars para cada tentativa de login
		// efetua logoff
		gSessao_Logado = false;
		gSessao_IDUser = -1;
		gSessao_Nome = F("");
		gSessao_Nivel = -1;
		gSessao_SaldoAtual = -1;

		gTelaRenderizada_ADMIN = false;	

		gModoOperacao = F("STANDBY");  
		gModoOperacao_SubTela = F("");						



		LogTerm(F("MAIN: Timeout de tela ADMIN NOVO CARD"));

		delay(1000);
		
	}





	if (gTelaRenderizada_ADMIN_NOVO_CARD == false)
	{


		TELA_Zera_BotaoGenericoTela();

		//TELA_PrintNoTerm_BotaoGenericoTela();




	
		// Cabecalho
		TELA_Render_Label(F("Administracao"),			  Green, 		 -1, 2, 0, 10, F("RIGHT"));
		TELA_Render_Label(F("Usuarios >> Novo Cartao"),   CinzaClaro,    -1, 1, 0, 60, F("RIGHT"));
		

		// Campos da tela
		
		TELA_Render_Label(F("Cartao:"), CinzaLabels, -1, 1, 210, 130, F(""));
		
		//TELA_Render_Label(gAdmin_ID_Cartao_Scan, White, 1, 350, 130, F(""));
		TELA_Render_Label(F("Identificado"), White, -1, 1, 350, 130, F(""));




		TELA_Render_Label(F("Nome:"), CinzaLabels, -1, 1, 210, 180, F(""));

		if (gAdmin_User_Nome == F(""))
		{
			TELA_Render_Label(F("[Em branco]"), CinzaClaro, -1, 1, 350, 180, F(""));
		}
		else
		{
			TELA_Render_Label(gAdmin_User_Nome, White, -1, 1, 350, 180, F(""));
		}



		TELA_Render_Label(F("Nivel:"), CinzaLabels, -1, 1, 210, 230, F(""));
		
		if (gAdmin_User_Nivel == F(""))
		{
			TELA_Render_Label(F("[Em branco]"), CinzaClaro, -1, 1, 350, 230, F(""));
		}
		else
		{
			TELA_Render_Label(gAdmin_User_Nivel, White, -1, 1, 350, 230, F(""));
		}




		TELA_Render_Label(F("CPF:"), CinzaLabels, -1, 1, 210, 280, F(""));
		
		if (gAdmin_User_CPF == F(""))
		{
			TELA_Render_Label(F("[Em branco]"), CinzaClaro, -1, 1, 350, 280, F(""));
		}
		else
		{
			TELA_Render_Label(gAdmin_User_CPF, White, -1, 1, 350, 280, F(""));
		}


		
		TELA_Render_Label(F("Saldo:"), CinzaLabels, -1, 1, 210, 330, F(""));

		if (gAdmin_User_Saldo == F(""))
		{
			TELA_Render_Label(F("[Em branco]"), CinzaClaro, -1, 1, 350, 330, F(""));
		}
		else
		{
			TELA_Render_Label(FormatNumber(gAdmin_User_Saldo, F("MONEY")), White, -1, 1, 350, 330, F(""));
		}

		


		
		//TELA_Render_BotaoGenerico(1, F("EDITAR_CARTAO"), F("Editar"), 0, White, Red, 110, 162, 70, 30);
		TELA_Render_BotaoGenerico(1, F("EDITAR_NOME"),   F("Editar"), 0, White, Red, 110, 182, 70, 30);
		TELA_Render_BotaoGenerico(2, F("EDITAR_NIVEL"),    F("Editar"), 0, White, Red, 110, 232, 70, 30);
		TELA_Render_BotaoGenerico(3, F("EDITAR_CPF"),    F("Editar"), 0, White, Red, 110, 282, 70, 30);
		TELA_Render_BotaoGenerico(4, F("EDITAR_SALDO"),  F("Editar"), 0, White, Red, 110, 332, 70, 30);

		TELA_Render_BotaoGenerico(5, F("CANCELAR"), F("CANCELAR"), 0, White, Blue, 210, 405, 150, 50);
		TELA_Render_BotaoGenerico(6, F("SALVAR"),   F("SALVAR"),   0, White, Blue, 450, 405, 150, 50);
	

		//TELA_PrintNoTerm_BotaoGenericoTela();


	}


	gTelaRenderizada_ADMIN_NOVO_CARD = true;

}



void TELA_Render_Interface_ADMIN_OUTROS()
{




	if (gTelaRenderizada_ADMIN_OUTROS == false)
	{
		// Inicia a contagem do tempo
		Timeout_TelaAtiva_time_inicio = millis();
	}
	else
	{
		if ((ctTIMEOUT_TELA_ADMIN / 1000) - Timeout_TelaAtiva_SegundosPassados <= 0)
		{
			Timeout_TelaAtiva_time_inicio = millis();

			gTelaRenderizada_ADMIN_OUTROS = false;

		}
	}


    Timeout_TelaAtiva_time_atual = millis();
    Timeout_TelaAtiva_time_tempo_passado = Timeout_TelaAtiva_time_atual - Timeout_TelaAtiva_time_inicio;

    Timeout_TelaAtiva_SegundosPassados = floor(Timeout_TelaAtiva_time_tempo_passado / 1000);


	// Renderiza os segundos
	if (String(ctTELA_HARDWARE) == String(F("ER-TFTM070-5")))
	{  		

	    if (Timeout_TelaAtiva_SegundosPassados != Timeout_Operacao_Last_SegundosPassados)
	    {	

				TELA_SetFontSize(0); 
				tft.setTextColor(CinzaLabels, RA8875_BLACK);
				tft.setCursor (640, 450);			
				tft.print (F("Tempo: ")); 

				tft.setCursor (700, 450);
				tft.print (String((ctTIMEOUT_TELA_ADMIN / 1000) - Timeout_TelaAtiva_SegundosPassados) + String(F(" segundos   "))); 


		} 
	}




	// sai se o tempo limite ocorrer
	if ((ctTIMEOUT_TELA_ADMIN / 1000) - Timeout_TelaAtiva_SegundosPassados <= 0)
	{


		// zera as vars para cada tentativa de login
		// efetua logoff
		gSessao_Logado = false;
		gSessao_IDUser = -1;
		gSessao_Nome = F("");
		gSessao_Nivel = -1;
		gSessao_SaldoAtual = -1;

		gTelaRenderizada_ADMIN = false;	

		gModoOperacao = F("STANDBY");  
		gModoOperacao_SubTela = F("");						



		LogTerm(F("MAIN: Timeout de tela ADMIN OUTROS"));

		delay(1000);
		
	}



	if (gTelaRenderizada_ADMIN_OUTROS == false)
	{

		if (ctTELA_HARDWARE == F("TERMINAL"))
		{  
			LogTerm(F("Selecione a opcao de administracao desejada:"));
		
		    LogTerm(F("99 - Sair"));		
		}


		if (String(ctTELA_HARDWARE) == String(F("ER-TFTM070-5")))
		{  		

			TELA_Zera_BotaoGenericoTela();

			TELA_SetFontSize(1); 

			// usuario logado
			tft.setTextColor(CinzaLabels);
			tft.setCursor (10, 440);			
			tft.print (F("Nome: ")); 


			tft.setTextColor(RA8875_WHITE);
			tft.setCursor (100, 440);			
			tft.print (gSessao_Nome); 



			// Cabecalho
			TELA_Render_Label(F("Administracao"), Green, 		 -1, 2, 0, 10, F("RIGHT"));
			TELA_Render_Label(F("Outros"),   	  CinzaClaro,    -1, 1, 0, 60, F("RIGHT"));

			// botao voltar
		    TELA_Render_BotaoGenerico(1, F("VOLTAR"), F("VOLTAR"), 1, White, Red, 10, 15, 130, 60);

		    // texto geral
			TELA_Texto_Centralizado(F("Escolha a sua opcao:"), F("AMARELO"), 2, 120);



			int _Espacamento = 25;
			int _Offset_X = 15;
			int _TamW = 134;
			int _TamH = 140;
			int _PosX = 0;
			int _PosY = 230;
			int _ContaBtn = 0;
			String _TxtBotao = F("");
			String _TAG_Botao = F("");


			_Offset_X = _Offset_X - _Espacamento;	// como o primeiro botao ja inclue um espacamento, eu subtraio ele para ter o offset real

			for (_ContaBtn = 1 ; _ContaBtn <= 4 ; _ContaBtn++)
			{
				_PosX = (_Espacamento * _ContaBtn) + (_TamW * (_ContaBtn - 1));

				switch (_ContaBtn)
				{
					case 1:
						_TxtBotao = F("Data e|Horario");
						_TAG_Botao = F("DATA_HORARIO");
						break;
					case 2:
						_TxtBotao = F("Reset");
						_TAG_Botao = F("RESET");
						break;
					case 3:
						_TxtBotao = F(" Modo |Debug");
						_TAG_Botao = F("MODO_DEBUG");
						break;
					case 4:
						_TxtBotao = F("Sangria");
						_TAG_Botao = F("SANGRIA");
						break;

				}


				TELA_Render_BotaoGenerico(_ContaBtn + 1, _TAG_Botao, _TxtBotao, 1, White, Blue, _Offset_X + _PosX, _PosY, _TamW, _TamH);
			}

			
			TELA_PrintNoTerm_BotaoGenericoTela();



		}

		gTelaRenderizada_ADMIN_OUTROS = true;

	}


}





void TELA_Render_Interface_ADMIN_SANGRIA()
{



	if (gTelaRenderizada_ADMIN_SANGRIA == false)
	{
		// Inicia a contagem do tempo
		Timeout_TelaAtiva_time_inicio = millis();
	}
	else
	{
		if ((ctTIMEOUT_TELA_ADMIN / 1000) - Timeout_TelaAtiva_SegundosPassados <= 0)
		{
			Timeout_TelaAtiva_time_inicio = millis();

			gTelaRenderizada_ADMIN_SANGRIA = false;

		}
	}


    Timeout_TelaAtiva_time_atual = millis();
    Timeout_TelaAtiva_time_tempo_passado = Timeout_TelaAtiva_time_atual - Timeout_TelaAtiva_time_inicio;

    Timeout_TelaAtiva_SegundosPassados = floor(Timeout_TelaAtiva_time_tempo_passado / 1000);


	// Renderiza os segundos
	if (String(ctTELA_HARDWARE) == String(F("ER-TFTM070-5")))
	{  		

	    if (Timeout_TelaAtiva_SegundosPassados != Timeout_Operacao_Last_SegundosPassados)
	    {	

				TELA_SetFontSize(0); 
				tft.setTextColor(CinzaLabels, RA8875_BLACK);
				tft.setCursor (240, 20);			
				tft.print (F("Tempo: ")); 

				tft.setCursor (300, 20);
				tft.print (String((ctTIMEOUT_TELA_ADMIN / 1000) - Timeout_TelaAtiva_SegundosPassados) + String(F(" segundos   "))); 

		} 
	}

	//LogTerm(String(F("(ctTIMEOUT_TELA_ADMIN / 1000) - SegundosPassados = ")) + String((ctTIMEOUT_TELA_ADMIN / 1000) - Timeout_TelaAtiva_SegundosPassados));



	// sai se o tempo limite ocorrer
	if ((ctTIMEOUT_TELA_ADMIN / 1000) - Timeout_TelaAtiva_SegundosPassados <= 0)
	{


		// zera as vars para cada tentativa de login
		// efetua logoff
		gSessao_Logado = false;
		gSessao_IDUser = -1;
		gSessao_Nome = F("");
		gSessao_Nivel = -1;
		gSessao_SaldoAtual = -1;

		
		TELA_LimpaTela();


		gTelaRenderizada_ADMIN_SANGRIA = false;	
	


		gModoOperacao = F("STANDBY");  
		gModoOperacao_SubTela = F("");	

		TELA_LimpaTela();


		LogTerm(F("MAIN: Timeout de tela Sangria"));

		delay(1000);
		
	}

	// ====================================================================



	if (gTelaRenderizada_ADMIN_SANGRIA== false)
	{

		

		if (ctTELA_HARDWARE == F("TERMINAL"))
		{  


			LogTerm(F("Selecione o chopp desejado e digite o numero correspondente no console:"));

		    // NumTorneira;DataCad;IDChopp;VolumeAtual;DataExpira;Ativa;NomeFromBanco;Tipo;Valor
		    for (int x = 0 ; x <= ctMAX_TORNEIRAS ; x++)
		    {

		        if (gaEngatados[x] != F(""))
		        {


		            String tmp_IDChopp = getValue(gaEngatados[x], ';', 2);
		            String tmp_Nome = getValue(gaEngatados[x], ';', 7);
		            String tmp_Tipo = getValue(gaEngatados[x], ';', 8);
		            String tmp_Valor = getValue(gaEngatados[x], ';', 9);
		            String tmp_Volume = getValue(gaEngatados[x], ';', 3);
		            //tmp_DataCad = getValue(gaEngatados[x], ';', 1);
		            //tmp_DataExp = getValue(gaEngatados[x], ';', 4);
		            String tmp_Ativa = getValue(gaEngatados[x], ';', 5);


		            LogTerm(  
		            		String(x + 1) + 
		            		String(F(" - ")) + 
		            		tmp_Nome + 
		            		String(F(" | R$ ")) + 
							tmp_Valor + 
							String(F(" / Litro ")) 
		                   );



		        }


		    }	

		    LogTerm(F("98 - Admin"));		
		    LogTerm(F("99 - Sair"));		
		}


		if (String(ctTELA_HARDWARE) == String(F("ER-TFTM070-5")))
		{  		




			// Cabecalho
			TELA_Render_Label(F("Administracao"), Green, -1 ,2, 0, 10, F("RIGHT"));
			TELA_Render_Label(F("Outros >> Sangria"),   CinzaClaro, -1 ,1, 0, 60, F("RIGHT"));


			TELA_Texto_Centralizado(F("SANGRIA: Escolha a sua torneira:"), F("VERDE"), 1, 117);



		
			// Botao VOLTAR ///////////////////////////////

		    int btnSair_PosAtual_X = 10;
			int btnSair_PosAtual_Y = 15;

			int btnSair_Size_W = 130;
			int btnSair_Size_H = 60;

			tft.fillRoundRect(btnSair_PosAtual_X, btnSair_PosAtual_Y, btnSair_Size_W, btnSair_Size_H, 8, Red);
		
			TELA_SetFontSize(1);
		    tft.setTextColor(RA8875_WHITE);
		    tft.setCursor (btnSair_PosAtual_X + (btnSair_Size_W / 2) - 55, btnSair_PosAtual_Y + 12); 
		    tft.print (F("VOLTAR"));	

		    //////////////////////////////////////



		    //////////////////////////////////////



		    // Inicia desenho do restante da tela e botoes


			gOffset_H = POSICAO_PADRAO_BTN_Y + 49;




		    // NumTorneira;DataCad;IDChopp;VolumeAtual;DataExpira;Ativa;NomeFromBanco;Tipo;Valor
		    for (int x = 0 ; x <= ctMAX_TORNEIRAS - 1 ; x++)
		    {

		        if (gaEngatados[x] != "")
		        {


		            String tmp_IDChopp = getValue(gaEngatados[x], ';', 2);
		            String tmp_Nome = getValue(gaEngatados[x], ';', 7);
		            String tmp_Tipo = getValue(gaEngatados[x], ';', 8);
		            String tmp_Valor = getValue(gaEngatados[x], ';', 9);
		            String tmp_Volume = getValue(gaEngatados[x], ';', 3);
		            //tmp_DataCad = getValue(gaEngatados[x], ';', 1);
		            //tmp_DataExp = getValue(gaEngatados[x], ';', 4);
		            String tmp_Ativa = getValue(gaEngatados[x], ';', 5);


		            TELA_Render_BotaoTorneira(x + 1, tmp_Nome, tmp_Tipo, String( FormatNumber(tmp_Valor, F("MONEY")) + String(F(" / Litro")) ), String(FormatNumber(tmp_Volume, F("")) + String(F(" litros")) ), F("AZUL"), F("TORNEIRA") );




		        }


		    }

		}

		gTelaRenderizada_ADMIN_SANGRIA = true;

	}

}


void TELA_Render_Interface_ADMIN_ADD_VALOR()
{

}

void TELA_Render_Interface_ADMIN_SUB_VALOR()
{

}

void TELA_Render_Interface_ADMIN_SET_SALDO()
{

}


void TELA_Render_Interface_OPERACAO()
{

	//LogTerm(F("== [Modo Atual: OPERACAO] ==");

	// Mostra tempo de timeout da tela de operacao -----------------------



	if (gTelaRenderizada_OPERACAO == false)
	{
		// Inicia a contagem do tempo
		Timeout_TelaAtiva_time_inicio = millis();
	}
	else
	{
		if ((ctTIMEOUT_TELA_OPERACAO / 1000) - Timeout_TelaAtiva_SegundosPassados <= 0)
		{
			Timeout_TelaAtiva_time_inicio = millis();

			gTelaRenderizada_OPERACAO = false;

		}
	}


    Timeout_TelaAtiva_time_atual = millis();
    Timeout_TelaAtiva_time_tempo_passado = Timeout_TelaAtiva_time_atual - Timeout_TelaAtiva_time_inicio;

    Timeout_TelaAtiva_SegundosPassados = floor(Timeout_TelaAtiva_time_tempo_passado / 1000);


	// Renderiza os segundos
	if (String(ctTELA_HARDWARE) == String(F("ER-TFTM070-5")))
	{  		

	    if (Timeout_TelaAtiva_SegundosPassados != Timeout_Operacao_Last_SegundosPassados)
	    {	

				TELA_SetFontSize(0); 
				tft.setTextColor(CinzaLabels, RA8875_BLACK);
				tft.setCursor (620, 10);			
				tft.print (F("Tempo: ")); 

				tft.setCursor (680, 10);
				tft.print (String((ctTIMEOUT_TELA_OPERACAO / 1000) - Timeout_TelaAtiva_SegundosPassados) + String(F(" segundos   "))); 

		} 
	}

	//LogTerm(String(F("(ctTIMEOUT_TELA_OPERACAO / 1000) - SegundosPassados = ")) + String((ctTIMEOUT_TELA_OPERACAO / 1000) - Timeout_TelaAtiva_SegundosPassados));



	// sai se o tempo limite ocorrer
	if ((ctTIMEOUT_TELA_OPERACAO / 1000) - Timeout_TelaAtiva_SegundosPassados <= 0)
	{


		// zera as vars para cada tentativa de login
		// efetua logoff
		gSessao_Logado = false;
		gSessao_IDUser = -1;
		gSessao_Nome = F("");
		gSessao_Nivel = -1;
		gSessao_SaldoAtual = -1;

		gTelaRenderizada_OPERACAO = false;	

		gModoOperacao = F("STANDBY");  
		gModoOperacao_SubTela = F("");						



		LogTerm(F("MAIN: Timeout de tela Operacao"));

		delay(1000);
		
	}

	// ====================================================================



	if (gTelaRenderizada_OPERACAO == false)
	{

		

		if (ctTELA_HARDWARE == F("TERMINAL"))
		{  


			LogTerm(F("Selecione o chopp desejado e digite o numero correspondente no console:"));

		    // NumTorneira;DataCad;IDChopp;VolumeAtual;DataExpira;Ativa;NomeFromBanco;Tipo;Valor
		    for (int x = 0 ; x <= ctMAX_TORNEIRAS ; x++)
		    {

		        if (gaEngatados[x] != F(""))
		        {


		            String tmp_IDChopp = getValue(gaEngatados[x], ';', 2);
		            String tmp_Nome = getValue(gaEngatados[x], ';', 7);
		            String tmp_Tipo = getValue(gaEngatados[x], ';', 8);
		            String tmp_Valor = getValue(gaEngatados[x], ';', 9);
		            String tmp_Volume = getValue(gaEngatados[x], ';', 3);
		            //tmp_DataCad = getValue(gaEngatados[x], ';', 1);
		            //tmp_DataExp = getValue(gaEngatados[x], ';', 4);
		            String tmp_Ativa = getValue(gaEngatados[x], ';', 5);


		            LogTerm(  
		            		String(x + 1) + 
		            		String(F(" - ")) + 
		            		tmp_Nome + 
		            		String(F(" | R$ ")) + 
							tmp_Valor + 
							String(F(" / Litro ")) 
		                   );



		        }


		    }	

		    LogTerm(F("98 - Admin"));		
		    LogTerm(F("99 - Sair"));		
		}


		if (String(ctTELA_HARDWARE) == String(F("ER-TFTM070-5")))
		{  		


			
			/*

			// Cabecalho logado ///////

			TELA_SetFontSize(1);

			tft.setTextColor(CinzaLabels);
			tft.setCursor (10, 10);			
			tft.print (F("Nome: ")); 
			tft.setCursor (10, 45);			
			tft.print (F("Saldo: "));  

			tft.setTextColor(RA8875_WHITE);
			tft.setCursor (120, 10);			
			tft.print (gSessao_Nome); 
			tft.setCursor (120, 45);	

			tft.print (FormatNumber(gSessao_SaldoAtual, F("MONEY")));  

			*/


			//////////////////////////////////////  

			TELA_Texto_Centralizado(F("Escolha a sua torneira:"), F("VERDE"), 2, 107);



		
			// Botao SAIR ///////////////////////////////

		    int btnSair_PosAtual_X = 10;
			int btnSair_PosAtual_Y = 15;

			int btnSair_Size_W = 130;
			int btnSair_Size_H = 60;

			tft.fillRoundRect(btnSair_PosAtual_X, btnSair_PosAtual_Y, btnSair_Size_W, btnSair_Size_H, 8, Red);
		
			TELA_SetFontSize(1);
		    tft.setTextColor(RA8875_WHITE);
		    tft.setCursor (btnSair_PosAtual_X + (btnSair_Size_W / 2) - 35, btnSair_PosAtual_Y + 12); 
		    tft.print (F("SAIR"));	

		    //////////////////////////////////////



		
			// Botao ADMIN ///////////////////////////////

		    int BtnAdminTransparente = 1;

		    int btnAdmin_PosAtual_X = 190;
			int btnAdmin_PosAtual_Y = 15;

			int btnAdmin_Size_W = 120;
			int btnAdmin_Size_H = 60;


			if (BtnAdminTransparente == 0)
			{
				tft.fillRoundRect(btnAdmin_PosAtual_X, btnAdmin_PosAtual_Y, btnAdmin_Size_W, btnAdmin_Size_H, 8, Red);
			
				TELA_SetFontSize(1);
			    tft.setTextColor(RA8875_WHITE);
			    tft.setCursor (btnAdmin_PosAtual_X + (btnAdmin_Size_W / 2) - 40, btnAdmin_PosAtual_Y + 12); 
			    tft.print (F("ADMIN"));				
			}


		    //////////////////////////////////////



		    // Inicia desenho do restante da tela e botoes


			gOffset_H = POSICAO_PADRAO_BTN_Y + 49;




		    // NumTorneira;DataCad;IDChopp;VolumeAtual;DataExpira;Ativa;NomeFromBanco;Tipo;Valor
		    for (int x = 0 ; x <= ctMAX_TORNEIRAS ; x++)
		    {

		        if (gaEngatados[x] != "")
		        {


		            String tmp_IDChopp = getValue(gaEngatados[x], ';', 2);
		            String tmp_Nome = getValue(gaEngatados[x], ';', 7);
		            String tmp_Tipo = getValue(gaEngatados[x], ';', 8);
		            String tmp_Valor = getValue(gaEngatados[x], ';', 9);
		            String tmp_Volume = getValue(gaEngatados[x], ';', 3);
		            //tmp_DataCad = getValue(gaEngatados[x], ';', 1);
		            //tmp_DataExp = getValue(gaEngatados[x], ';', 4);
		            String tmp_Ativa = getValue(gaEngatados[x], ';', 5);


		            TELA_Render_BotaoTorneira(x + 1, tmp_Nome, tmp_Tipo, String( FormatNumber(tmp_Valor, F("MONEY")) + String(F(" / Litro")) ), String(FormatNumber(tmp_Volume, F("")) + String(F(" litros")) ), F("AZUL"), F("TORNEIRA") );




		        }


		    }

		}

		gTelaRenderizada_OPERACAO = true;

	}





    




}







void TELA_VerificaTouch_STANDBY()
{

	if (ctTELA_HARDWARE == F("TERMINAL"))
	{ 

		String retConsole;

		retConsole = ReadConsoleInput();

		if (retConsole != F(""))
		{
			LogTerm(String(F("Opcao selecionada: ")) + retConsole);

			gTelaRenderizada_STANDBY = false;

			//gModoOperacao = F("LOGIN");
			gModoOperacao = "OPERACAO";
			//gModoOperacao = "DEBUG";


     
			//delay(500);		
		}

	}

	if (String(ctTELA_HARDWARE) == String(F("ER-TFTM070-5")))
	{  

		if (TELA_touchDetect())
		{


			// Esquema de DEBounce ---- inicio

			gBounce_ContaClick++;

			TELA_touchReadPixel(&gTouch_X, &gTouch_Y);

			//LogTerm(String(F("TELA_VerificaTouch_STANDBY - gBounce_ContaClick = ")) + String(gBounce_ContaClick) + String(F(" / gBounce_time_tempo_passado = ")) + String(gBounce_time_tempo_passado) + String(F(" / gBounce_SegundosPassados = ")) + String(gBounce_SegundosPassados));


			//if ((gBounce_ContaClick == 1) && (gBounce_time_tempo_passado <= ctBOUNCE_SENSIB_BOTAO))
			if (gBounce_ContaClick == 1)
			{
				// Local onde deve ocorrer o evento do clique. Ocorrera apenas 1 vez --------



				/*
				gTelaRenderizada_STANDBY = false;

				//gModoOperacao = F("LOGIN");
				gModoOperacao = F("TECLADO");
				gModoOperacao_SubTela = F("TEC_ALPHA_TESTE_STANDBY");

			    TELA_LimpaTela();

				delay(200);  
				*/


			
				// original para ir para operacao

				gTelaRenderizada_STANDBY = false;

				//gModoOperacao = F("LOGIN");
				gModoOperacao = F("OPERACAO");


			    TELA_LimpaTela();

				delay(500);  
			


				// -----------------------------------

				gBounce_time_inicio = millis();
			}


			gBounce_time_atual = millis();
			gBounce_time_tempo_passado = gBounce_time_atual - gBounce_time_inicio;

			gBounce_SegundosPassados = floor(gBounce_time_tempo_passado / 1000);

			//LogTerm(gBounce_time_tempo_passado);

			if (gBounce_SegundosPassados != gBounce_Last_SegundosPassados)
			{
				//LogTerm(time_tempo_passado);
			}



			if (gBounce_time_tempo_passado >= ctBOUNCE_SENSIB_BOTAO)
			{

				gBounce_ContaClick = 0;		

			}

			gBounce_Last_SegundosPassados = gBounce_SegundosPassados;

			// Esquema de DEBounce ---- FIM



		


		}

	}

}







void TELA_VerificaTouch_LOGIN()
{

	if (ctTELA_HARDWARE == F("TERMINAL"))
	{ 

		String retConsole;

		retConsole = ReadConsoleInput();

		if (retConsole != F(""))
		{
			LogTerm(String(F("Opcao selecionada: ")) + retConsole);	
		}


		if (retConsole.toInt() == 1)
		{
			//LogTerm(F("LEITOR BIOMETRICO SELECIONADO");
		}

		if (retConsole.toInt() == 2)
		{
			//LogTerm(F("LEITOR RFID SELECIONADO VIA TERMINAL");


			gModoOperacao_SubTela = F("LER_RFID");


			gTelaRenderizada_LOGIN = false;



			
		}

		if (retConsole.toInt() == 3)
		{
			gModoOperacao = F("DEBUG");
			gTelaRenderizada_LOGIN = false;

			//LogTerm(F("TECLADO NUM SELECIONADO");
			//LogTerm(F("TECLADO ALFA SELECIONADO");


			delay(500);   
		}

		if (retConsole.toInt() == 4)
		{
			gModoOperacao = F("ADMIN"); 
			gTelaRenderizada_LOGIN = false;

			//LogTerm(F("ADMIN SELECIONADO");

			delay(500);    
		}

	}




	if (String(ctTELA_HARDWARE) == String(F("ER-TFTM070-5")))
	{  

		//tft.changeMode(GRAPHIC);

		if (TELA_touchDetect())
		{


			TELA_touchReadPixel(&gTouch_X, &gTouch_Y);

			//LogTerm(F("TELA_VerificaTouch_LOGIN"));
			//TELA_LogTerm_XY();
			

			// murch
			//gTouch_X = 800 - gTouch_X;
			//gTouch_Y = 480 - gTouch_Y;

			//TELA_LogTerm_XY(); 

			//botao 1:
			if (gTouch_X >= gOffset_W && gTouch_X <= gTamBotao_W + gOffset_W)  
			{

				if (gTouch_Y >= gOffset_H && gTouch_Y <= gTamBotao_H + gOffset_H) 
				{

					LogTerm(F("LEITOR BIOMETRICO SELECIONADO"));
					TELA_Texto(F("LEITOR BIOMETRICO SELECIONADO"), F("BRANCO"));

	

				}

			}



			//botao 2:
			if (gTouch_X >= 2 * gOffset_W + gTamBotao_W && gTouch_X <= 2 * gOffset_W + 2 * gTamBotao_W )  
			{

				if (gTouch_Y >= gOffset_H && gTouch_Y <= gTamBotao_H + gOffset_H) 
				{

					//TELA_Texto("LEITOR RFID SELECIONADO VIA TELA", "AZUL");


					//LogTerm(F("LEITOR RFID SELECIONADO VIA TELA"));

					//LogTerm(F("Aproxime o cartao da leitora RFID ...");

					gModoOperacao_SubTela = F("LER_RFID");


					gTelaRenderizada_LOGIN = false;

					TELA_LimpaTela();

					delay(500);   





				}

			}


			//botao 3:
			if (gTouch_X >= 3 * gOffset_W + 2 * gTamBotao_W && gTouch_X <= 3 * gOffset_W + 3 * gTamBotao_W )  
			{

				if (gTouch_Y >= gOffset_H && gTouch_Y <= gTamBotao_H + gOffset_H) 
				{
					
					gTelaRenderizada_LOGIN = false;
					gModoOperacao = F("DEBUG"); 

					//TELA_Texto(F("TECLADO NUM SELECIONADO"), F("MAGENTA"));
					TELA_Texto(F("TECLADO ALFA SELECIONADO"), F("MAGENTA"));

					TELA_LimpaTela();

					delay(500);          
				}

			}

			/*

			//botao ADMIN:
			if (gTouch_X >= 700 && gTouch_X <= 700 + 100)  
			{

				if (gTouch_Y >= 0 && gTouch_Y <= 0 + 100) 
				{


					gTelaRenderizada_LOGIN = false;
					gModoOperacao = "ADMIN"; 

					TELA_Texto("ADMIN SELECIONADO", "MAGENTA");

					TELA_LimpaTela();

					delay(500);          
				}

			}

			*/


		}

	}


}





void TELA_VerificaTouch_ADMIN()
{


	if (String(ctTELA_HARDWARE) == String(F("ER-TFTM070-5")))
	{  

		if (TELA_touchDetect())
		{



			TELA_touchReadPixel(&gTouch_X, &gTouch_Y);
		
			/*


			/*
		    gaBotoesGenTela[Index - 1] = String(Index) 	+ String(F(";")) + 
		    							 TAG 			+ String(F(";")) + 
		    							 Texto 			+ String(F(";")) + 
		    							 String(PosX) 	+ String(F(";")) + 
		    							 String(PosY) 	+ String(F(";")) + 
		    							 String(W) 		+ String(F(";")) + 
		    							 String(H);

			*/



			for (int ContaBotaoGen = 1 ; ContaBotaoGen <= ctMAX_BOTOES_GEN_TELA; ContaBotaoGen++)
			{
				if (gaBotoesGenTela[ContaBotaoGen - 1] != F(""))
				{
				

					//LogTerm( String(F("gaBotoesGenTela[")) + String(ContaBotaoGen - 1) + String(F("] = ")) + gaBotoesGenTela[ContaBotaoGen - 1] );

					String btnGen_TAG = 	getValue(gaBotoesGenTela[ContaBotaoGen - 1], ';', 1);
					String btnGen_PosX = 	getValue(gaBotoesGenTela[ContaBotaoGen - 1], ';', 3);
					String btnGen_PosY = 	getValue(gaBotoesGenTela[ContaBotaoGen - 1], ';', 4);
					String btnGen_W = 		getValue(gaBotoesGenTela[ContaBotaoGen - 1], ';', 5);
					String btnGen_H = 		getValue(gaBotoesGenTela[ContaBotaoGen - 1], ';', 6);

					/*

					LogTerm(String(F("btnGen_TAG = ")) + btnGen_TAG);
					LogTerm(String(F("btnGen_PosX = ")) + btnGen_PosX);
					LogTerm(String(F("btnGen_PosY = ")) + btnGen_PosY);
					LogTerm(String(F("btnGen_W = ")) + btnGen_W);
					LogTerm(String(F("btnGen_H = ")) + btnGen_H);
					
					LogTerm(String(F("===============================")));

					*/



					//botao 1:
					if (gTouch_X >= btnGen_PosX.toInt() && gTouch_X <= btnGen_W.toInt() + btnGen_PosX.toInt())  
					{

						if (gTouch_Y >= btnGen_PosY.toInt() && gTouch_Y <= btnGen_H.toInt() + btnGen_PosY.toInt()) 
						{


							// Esquema de DEBounce ---- inicio

							gBounce_ContaClick++;
							

							if (gBounce_ContaClick == 1)
							{
								// Local onde deve ocorrer o evento do clique. Ocorrera apenas 1 vez --------

			
								LogTerm(String(F("BOTAO GENERICO ")) + String(ContaBotaoGen) + String(F(" APERTADO")) + String(F(" (")) + btnGen_TAG + String(F(")"))  );



								if (btnGen_TAG == F("SAIR"))
								{

									TELA_LimpaTela();

									// zera as vars para cada tentativa de login
									// efetua logoff
									gSessao_Logado = false;
									gSessao_IDUser = -1;
									gSessao_Nome = F("");
									gSessao_Nivel = -1;
									gSessao_SaldoAtual = -1;

									gTelaRenderizada_ADMIN = false;	

									gModoOperacao = F("STANDBY");  
									gModoOperacao_SubTela = F("");						
								
									LogTerm(F("MAIN: Usuario clicou em SAIR"));
								}


								if (btnGen_TAG == F("USUARIOS"))
								{

									TELA_LimpaTela();

									gModoOperacao_SubTela = F("ADMIN_USUARIOS");

									gTelaRenderizada_ADMIN = false;
									gTelaRenderizada_ADMIN_USUARIOS = false;

									delay(500);  
								}


								if (btnGen_TAG == F("OUTROS"))
								{

									TELA_LimpaTela();

									gModoOperacao_SubTela = F("ADMIN_OUTROS");

									gTelaRenderizada_ADMIN = false;
									gTelaRenderizada_ADMIN_OUTROS = false;

									delay(500);  
								}

									

	
								// -----------------------------------

								gBounce_time_inicio = millis();

							}


							gBounce_time_atual = millis();
							gBounce_time_tempo_passado = gBounce_time_atual - gBounce_time_inicio;

							gBounce_SegundosPassados = floor(gBounce_time_tempo_passado / 1000);

							//LogTerm(gBounce_time_tempo_passado);

							if (gBounce_SegundosPassados != gBounce_Last_SegundosPassados)
							{
								//LogTerm(time_tempo_passado);
							}



							if (gBounce_time_tempo_passado >= ctBOUNCE_SENSIB_BOTAO)
							{
								gBounce_ContaClick = 0;	
							}

							gBounce_Last_SegundosPassados = gBounce_SegundosPassados;

							// Esquema de DEBounce ---- FIM



						}

					}


				}
			}	    							 


		}

	}


}




void TELA_VerificaTouch_ADMIN_USUARIOS()
{

	if (String(ctTELA_HARDWARE) == String(F("ER-TFTM070-5")))
	{  

		if (TELA_touchDetect())
		{

			TELA_touchReadPixel(&gTouch_X, &gTouch_Y);


			for (int ContaBotaoGen = 1 ; ContaBotaoGen <= ctMAX_BOTOES_GEN_TELA; ContaBotaoGen++)
			{
				if (gaBotoesGenTela[ContaBotaoGen - 1] != F(""))
				{
				
					//LogTerm( String(F("gaBotoesGenTela[")) + String(ContaBotaoGen - 1) + String(F("] = ")) + gaBotoesGenTela[ContaBotaoGen - 1] );

					String btnGen_TAG = 	getValue(gaBotoesGenTela[ContaBotaoGen - 1], ';', 1);
					String btnGen_PosX = 	getValue(gaBotoesGenTela[ContaBotaoGen - 1], ';', 3);
					String btnGen_PosY = 	getValue(gaBotoesGenTela[ContaBotaoGen - 1], ';', 4);
					String btnGen_W = 		getValue(gaBotoesGenTela[ContaBotaoGen - 1], ';', 5);
					String btnGen_H = 		getValue(gaBotoesGenTela[ContaBotaoGen - 1], ';', 6);



					if (gTouch_X >= btnGen_PosX.toInt() && gTouch_X <= btnGen_W.toInt() + btnGen_PosX.toInt())  
					{

						if (gTouch_Y >= btnGen_PosY.toInt() && gTouch_Y <= btnGen_H.toInt() + btnGen_PosY.toInt()) 
						{


							// Esquema de DEBounce ---- inicio

							gBounce_ContaClick++;
							

							if (gBounce_ContaClick == 1)
							{
								// Local onde deve ocorrer o evento do clique. Ocorrera apenas 1 vez --------

			
								LogTerm(String(F("BOTAO GENERICO ")) + String(ContaBotaoGen) + String(F(" APERTADO")) + String(F(" (")) + btnGen_TAG + String(F(")"))  );


								//BOTAO SAIR
								if (btnGen_TAG == F("VOLTAR"))
								{

									TELA_LimpaTela();

									//gModoOperacao = F("ADMIN");
									gModoOperacao_SubTela = F("");		

									gTelaRenderizada_ADMIN = false;	
									gTelaRenderizada_ADMIN_USUARIOS = false;	

									/*
									// zera as vars para cada tentativa de login
									// efetua logoff
									gSessao_Logado = false;
									gSessao_IDUser = -1;
									gSessao_Nome = F("");
									gSessao_Nivel = -1;
									gSessao_SaldoAtual = -1;

									gTelaRenderizada_ADMIN = false;	

									gModoOperacao = F("STANDBY");  
									gModoOperacao_SubTela = F("");						
								
									LogTerm(F("MAIN: Usuario clicou em SAIR"));

									*/

								}


								// BOTAO ADICIONAR CARTAO
								if (btnGen_TAG == F("NOVO_CARTAO"))
								{

									TELA_LimpaTela();

									gModoOperacao = F("LOGIN");
									gModoOperacao_SubTela = F("LER_RFID_ADMIN_NOVO_CARD");

									gTelaRenderizada_ADMIN_USUARIOS = false;

									/*
									gModoOperacao_SubTela = F("ADMIN_NOVO_CARD");

									gTelaRenderizada_ADMIN_USUARIOS = false;
									gTelaRenderizada_ADMIN_NOVO_CARD = false;
								
									TELA_LimpaTela();
									*/

									//delay(500);  
								}


								// BOTAO ADICIONAR VALOR
								if (btnGen_TAG == F("ADD_VALOR"))
								{
									TELA_LimpaTela();

									gModoOperacao = F("LOGIN");
									gModoOperacao_SubTela = F("LER_RFID_ADMIN_ADD_VALOR");

									gTelaRenderizada_ADMIN_USUARIOS = false;
								}

								// BOTAO SUBTRAIR VALOR
								if (btnGen_TAG == F("SUB_VALOR"))
								{
									TELA_LimpaTela();

									gModoOperacao = F("LOGIN");
									gModoOperacao_SubTela = F("LER_RFID_ADMIN_SUB_VALOR");

									gTelaRenderizada_ADMIN_USUARIOS = false;
								}

								// BOTAO DEFINIR SALDO
								if (btnGen_TAG == F("SET_SALDO"))
								{
									TELA_LimpaTela();

									gModoOperacao = F("LOGIN");
									gModoOperacao_SubTela = F("LER_RFID_ADMIN_SET_SALDO");

									gTelaRenderizada_ADMIN_USUARIOS = false;
								}

								// BOTAO TROCAR / CANCELAR CARTAO
								if (btnGen_TAG == F("TROCA_CARTAO"))
								{

								}								

	
								// -----------------------------------

								gBounce_time_inicio = millis();

							}


							gBounce_time_atual = millis();
							gBounce_time_tempo_passado = gBounce_time_atual - gBounce_time_inicio;

							gBounce_SegundosPassados = floor(gBounce_time_tempo_passado / 1000);

							//LogTerm(gBounce_time_tempo_passado);

							if (gBounce_SegundosPassados != gBounce_Last_SegundosPassados)
							{
								//LogTerm(time_tempo_passado);
							}



							if (gBounce_time_tempo_passado >= ctBOUNCE_SENSIB_BOTAO)
							{
								gBounce_ContaClick = 0;	
							}

							gBounce_Last_SegundosPassados = gBounce_SegundosPassados;

							// Esquema de DEBounce ---- FIM



						}

					}


				}
			}	    							 


		}

	}


}




void TELA_VerificaTouch_ADMIN_CHOPPS()
{

}

void TELA_VerificaTouch_ADMIN_ENGATADOS()
{

}

void TELA_VerificaTouch_ADMIN_OUTROS()
{

	if (String(ctTELA_HARDWARE) == String(F("ER-TFTM070-5")))
	{  

		if (TELA_touchDetect())
		{

			TELA_touchReadPixel(&gTouch_X, &gTouch_Y);


			for (int ContaBotaoGen = 1 ; ContaBotaoGen <= ctMAX_BOTOES_GEN_TELA; ContaBotaoGen++)
			{
				if (gaBotoesGenTela[ContaBotaoGen - 1] != F(""))
				{
				
					//LogTerm( String(F("gaBotoesGenTela[")) + String(ContaBotaoGen - 1) + String(F("] = ")) + gaBotoesGenTela[ContaBotaoGen - 1] );

					String btnGen_TAG = 	getValue(gaBotoesGenTela[ContaBotaoGen - 1], ';', 1);
					String btnGen_PosX = 	getValue(gaBotoesGenTela[ContaBotaoGen - 1], ';', 3);
					String btnGen_PosY = 	getValue(gaBotoesGenTela[ContaBotaoGen - 1], ';', 4);
					String btnGen_W = 		getValue(gaBotoesGenTela[ContaBotaoGen - 1], ';', 5);
					String btnGen_H = 		getValue(gaBotoesGenTela[ContaBotaoGen - 1], ';', 6);



					if (gTouch_X >= btnGen_PosX.toInt() && gTouch_X <= btnGen_W.toInt() + btnGen_PosX.toInt())  
					{

						if (gTouch_Y >= btnGen_PosY.toInt() && gTouch_Y <= btnGen_H.toInt() + btnGen_PosY.toInt()) 
						{


							// Esquema de DEBounce ---- inicio

							gBounce_ContaClick++;
							

							if (gBounce_ContaClick == 1)
							{
								// Local onde deve ocorrer o evento do clique. Ocorrera apenas 1 vez --------

			
								LogTerm(String(F("BOTAO GENERICO ")) + String(ContaBotaoGen) + String(F(" APERTADO")) + String(F(" (")) + btnGen_TAG + String(F(")"))  );


								//BOTAO SAIR
								if (btnGen_TAG == F("VOLTAR"))
								{

									TELA_LimpaTela();

									//gModoOperacao = F("ADMIN");
									gModoOperacao_SubTela = F("");		

									gTelaRenderizada_ADMIN = false;	
									gTelaRenderizada_ADMIN_OUTROS = false;	

									/*
									// zera as vars para cada tentativa de login
									// efetua logoff
									gSessao_Logado = false;
									gSessao_IDUser = -1;
									gSessao_Nome = F("");
									gSessao_Nivel = -1;
									gSessao_SaldoAtual = -1;

									gTelaRenderizada_ADMIN = false;	

									gModoOperacao = F("STANDBY");  
									gModoOperacao_SubTela = F("");						
								
									LogTerm(F("MAIN: Usuario clicou em SAIR"));

									*/

								}


								// BOTAO DATA_HORARIO
								if (btnGen_TAG == F("DATA_HORARIO"))
								{

									//TELA_LimpaTela();

									//gModoOperacao = F("LOGIN");
									//gModoOperacao_SubTela = F("LER_RFID_ADMIN_NOVO_CARD");

									//gTelaRenderizada_ADMIN_USUARIOS = false;

					
								}


								// BOTAO RESET
								if (btnGen_TAG == F("RESET"))
								{

									TELA_LimpaTela();

									TELA_Render_MsgBox(F("Reiniciar Sitema"), F("Reiniciando sistema..."));

									delay(100);

									ResetArduino();
								}

								// BOTAO MODO_DEBUG
								if (btnGen_TAG == F("MODO_DEBUG"))
								{

								}

								// BOTAO SANGRIA
								if (btnGen_TAG == F("SANGRIA"))
								{
									TELA_LimpaTela();

		
									gModoOperacao_SubTela = F("ADMIN_SANGRIA");


									gTelaRenderizada_ADMIN_OUTROS = false;
									gTelaRenderizada_ADMIN_SANGRIA = false;
								}

				
	
								// -----------------------------------

								gBounce_time_inicio = millis();

							}


							gBounce_time_atual = millis();
							gBounce_time_tempo_passado = gBounce_time_atual - gBounce_time_inicio;

							gBounce_SegundosPassados = floor(gBounce_time_tempo_passado / 1000);

							//LogTerm(gBounce_time_tempo_passado);

							if (gBounce_SegundosPassados != gBounce_Last_SegundosPassados)
							{
								//LogTerm(time_tempo_passado);
							}



							if (gBounce_time_tempo_passado >= ctBOUNCE_SENSIB_BOTAO)
							{
								gBounce_ContaClick = 0;	
							}

							gBounce_Last_SegundosPassados = gBounce_SegundosPassados;

							// Esquema de DEBounce ---- FIM



						}

					}


				}
			}	    							 


		}

	}




}


void TELA_VerificaTouch_ADMIN_NOVO_CARD()
{

	if (String(ctTELA_HARDWARE) == String(F("ER-TFTM070-5")))
	{  

		if (TELA_touchDetect())
		{


			//TELA_LogTerm_XY();
			//LogTerm(F("TELA_VerificaTouch_ADMIN_NOVO_CARD");

			TELA_touchReadPixel(&gTouch_X, &gTouch_Y);
		
			/*


			/*
		    gaBotoesGenTela[Index - 1] = String(Index) 	+ String(F(";")) + 
		    							 TAG 			+ String(F(";")) + 
		    							 Texto 			+ String(F(";")) + 
		    							 String(PosX) 	+ String(F(";")) + 
		    							 String(PosY) 	+ String(F(";")) + 
		    							 String(W) 		+ String(F(";")) + 
		    							 String(H);

			*/



			for (int ContaBotaoGen = 1 ; ContaBotaoGen <= ctMAX_BOTOES_GEN_TELA; ContaBotaoGen++)
			{
				if (gaBotoesGenTela[ContaBotaoGen - 1] != F(""))
				{
				

					//LogTerm( String(F("gaBotoesGenTela[")) + String(ContaBotaoGen - 1) + String(F("] = ")) + gaBotoesGenTela[ContaBotaoGen - 1] );

					String btnGen_TAG = 	getValue(gaBotoesGenTela[ContaBotaoGen - 1], ';', 1);
					String btnGen_PosX = 	getValue(gaBotoesGenTela[ContaBotaoGen - 1], ';', 3);
					String btnGen_PosY = 	getValue(gaBotoesGenTela[ContaBotaoGen - 1], ';', 4);
					String btnGen_W = 		getValue(gaBotoesGenTela[ContaBotaoGen - 1], ';', 5);
					String btnGen_H = 		getValue(gaBotoesGenTela[ContaBotaoGen - 1], ';', 6);

					/*

					LogTerm(String(F("btnGen_TAG = ")) + btnGen_TAG);
					LogTerm(String(F("btnGen_PosX = ")) + btnGen_PosX);
					LogTerm(String(F("btnGen_PosY = ")) + btnGen_PosY);
					LogTerm(String(F("btnGen_W = ")) + btnGen_W);
					LogTerm(String(F("btnGen_H = ")) + btnGen_H);
					
					LogTerm(String(F("===============================")));

					*/



					//botao 1:
					if (gTouch_X >= btnGen_PosX.toInt() && gTouch_X <= btnGen_W.toInt() + btnGen_PosX.toInt())  
					{

						if (gTouch_Y >= btnGen_PosY.toInt() && gTouch_Y <= btnGen_H.toInt() + btnGen_PosY.toInt()) 
						{




							// Esquema de DEBounce ---- inicio

							gBounce_ContaClick++;
							

							if (gBounce_ContaClick == 1)
							{
								// Local onde deve ocorrer o evento do clique. Ocorrera apenas 1 vez --------

			
								LogTerm(String(F("BOTAO GENERICO ")) + String(ContaBotaoGen) + String(F(" APERTADO")) + String(F(" (")) + btnGen_TAG + String(F(")"))  );



								if (btnGen_TAG == F("EDITAR_NOME"))
								{
									
									gTelaRenderizada_ADMIN_NOVO_CARD = false;
									gTelaRenderizada_TecAlfa = false;
									gTelaRenderizada_TECLADO = false;

									//TELA_RenderTecUnificado_ALFA();

									gTecladoAlfa_ValAtual = gAdmin_User_Nome;

									gModoOperacao = F("TECLADO");
									gModoOperacao_SubTela = F("TECLADO_ADMIN_USER_NOME");	

									TELA_LimpaTela();								

								}

								if (btnGen_TAG == F("EDITAR_NIVEL"))
								{

									gTelaRenderizada_ADMIN_NOVO_CARD = false;
									gTelaRenderizada_TecNum = false;
									gTelaRenderizada_TECLADO = false;

									gTecladoNum_ValAtual = gAdmin_User_Nivel;

									gModoOperacao = F("TECLADO");
									gModoOperacao_SubTela = F("TECLADO_ADMIN_USER_NIVEL");	
									
									TELA_LimpaTela();

								}

								if (btnGen_TAG == F("EDITAR_CPF"))
								{

									gTelaRenderizada_ADMIN_NOVO_CARD = false;
									gTelaRenderizada_TecNum = false;
									gTelaRenderizada_TECLADO = false;

									gTecladoNum_ValAtual = gAdmin_User_CPF;

									gModoOperacao = F("TECLADO");
									gModoOperacao_SubTela = F("TECLADO_ADMIN_USER_CPF");	
									
									TELA_LimpaTela();

								}

								if (btnGen_TAG == F("EDITAR_SALDO"))
								{

									gTelaRenderizada_ADMIN_NOVO_CARD = false;
									gTelaRenderizada_TecNum = false;
									gTelaRenderizada_TECLADO = false;

									gTecladoNum_ValAtual = gAdmin_User_Saldo;

									gModoOperacao = F("TECLADO");
									gModoOperacao_SubTela = F("TECLADO_ADMIN_USER_SALDO");	

									TELA_LimpaTela();

								}

								if (btnGen_TAG == F("CANCELAR"))
								{

									gAdmin_ID_Cartao_Scan = F("");
									gAdmin_User_Nome = F("");
									gAdmin_User_CPF = F("");
									gAdmin_User_Nivel = F("");									
									gAdmin_User_Saldo = F("");
									gAdmin_User_IDUser = F("");	
									gAdmin_User_Datacad = F("");	

 
									gModoOperacao_SubTela = F("ADMIN_USUARIOS");

									gTelaRenderizada_ADMIN_USUARIOS = false;
									gTelaRenderizada_ADMIN_NOVO_CARD = false;

									gTelaRenderizada_TecAlfa = false;
									gTelaRenderizada_TecNum = false;
									gTelaRenderizada_TECLADO = false;

									TELA_LimpaTela();

									//delay(500);  

								}

								if (btnGen_TAG == F("SALVAR"))
								{

									bool _Envia = true;
									String _txtErro = F("Os seguintes campos nao podem estar em  branco: ");

									if (gAdmin_ID_Cartao_Scan == F(""))
									{
										_Envia = false;
										_txtErro += String(F("ID do Cartao, "));
									}

									if (gAdmin_User_Nome == F(""))
									{
										_Envia = false;
										_txtErro += String(F("Nome, "));
									}

									if (gAdmin_User_Nivel == F(""))
									{
										_Envia = false;
										_txtErro += String(F("Nivel, "));
									}


									if (gAdmin_User_CPF == F(""))
									{
										_Envia = false;
										_txtErro += String(F("CPF, "));
									}


									if (gAdmin_User_Saldo == F(""))
									{
										_Envia = false;
										_txtErro += String(F("Saldo Inicial, "));
									}

		
									if (_Envia == true)
									{

										// rotina de criacao de novo usuario e login

										String IDUserMax = F("");
										String IDUserMax_Novo = F("");
										String retFunc = F("");

										gAdmin_User_Datacad = Now();

										bool _ContinuaCriacao = false;

										// oculta os botoes
										// oculta cancelar
										tft.fillRoundRect(200, 400, 160, 60, 8, Black);	
										// oculta salvar
										tft.fillRoundRect(440, 400, 160, 60, 8, Black);	

										TELA_Render_MsgBox(F("Novo Usuario"), String(F("Cadastrando novo usuario. Por favor, aguarde..."))) ;



										// cria usuario

										// pega iduser max atual

										retFunc = BANCO_GetIDUserMax();

										// testa erro e avisa e sai se ocorrer

										// erro de sd
										if (retFunc.substring(0, 2) == F("-1"))
										{
											TELA_LimpaTela();

											gTelaRenderizada_MSGBOX = false; 

											TELA_Render_MsgBox(F("Erro Cartao SD"), F("Ocorreu um erro ao acessar o cartao SD. O usuario nao foi cadastrado"));

											delay(6000);

											TELA_LimpaTela();

											gTelaRenderizada_MSGBOX = false;
											gTelaRenderizada_ADMIN_NOVO_CARD = false;

											gTelaRenderizada_TecAlfa = false;
											gTelaRenderizada_TecNum = false;	
											gTelaRenderizada_TECLADO = false;												
										}												

										// arquivo nao existe
										if (retFunc.substring(0, 2) == F("-2"))
										{
											gTelaRenderizada_MSGBOX = false; 
											
											TELA_Render_MsgBox(F("Arquivo nao localizado"), F("O arquivo de IDUserMax nao foi localizado"));

											delay(6000);

											TELA_LimpaTela();

											gTelaRenderizada_MSGBOX = false;
											gTelaRenderizada_ADMIN_NOVO_CARD = false;

											gTelaRenderizada_TecAlfa = false;
											gTelaRenderizada_TecNum = false;	
											gTelaRenderizada_TECLADO = false;	

										}												

										// sucesso
										if (retFunc.substring(0, 1) == F("1"))
										{
											IDUserMax = retFunc.substring(2);
											IDUserMax_Novo = String(IDUserMax.toInt() + 1);

											_ContinuaCriacao = true;

											LogTerm(String(F("IDUserMax localizado: ")) + IDUserMax);
											//LogTerm(String(F("IDUserMax_Novo: ")) + IDUserMax_Novo);

										}												

										if (_ContinuaCriacao == true)
										{


											// atualiza iduser max atual
											retFunc = BANCO_AtualizaMaxValue(IDUserMax_Novo, F("IDUSER"));

											// testa erro e avisa e sai se ocorrer										

											// sucesso
											if (retFunc.substring(0, 1) != F("1"))
											{
												gTelaRenderizada_MSGBOX = false; 
											
												TELA_Render_MsgBox(F("Erro ao atualizar Max"), F("Ocorreu um erro ao atualizar o valor Max"));

												delay(6000);

												TELA_LimpaTela();

												gTelaRenderizada_MSGBOX = false;
												gTelaRenderizada_ADMIN_NOVO_CARD = false;

												gTelaRenderizada_TecAlfa = false;
												gTelaRenderizada_TecNum = false;	
												gTelaRenderizada_TECLADO = false;	

												_ContinuaCriacao = false;

											}	
											
										}




										if (_ContinuaCriacao == true)
										{

											// efetivamente cria o usuario



											retFunc = SD_CreateNewUserFile(IDUserMax_Novo, 
																			gAdmin_User_Nome, 
																			gAdmin_User_Nivel, 
																			gAdmin_User_CPF, 
																			gAdmin_User_Saldo,
																			gAdmin_User_Datacad);
										


											// testa erro e avisa e sai se ocorrer
											if (retFunc.substring(0, 1) != F("1"))
											{
												gTelaRenderizada_MSGBOX = false; 
											
												TELA_Render_MsgBox(F("Erro ao cadastrar usuario"), F("Ocorreu um erro ao cadastrar o novo usuario"));

												delay(6000);

												TELA_LimpaTela();

												gTelaRenderizada_MSGBOX = false;
												gTelaRenderizada_ADMIN_NOVO_CARD = false;

												gTelaRenderizada_TecAlfa = false;
												gTelaRenderizada_TecNum = false;	
												gTelaRenderizada_TECLADO = false;	

												_ContinuaCriacao = false;
											}	
										

										}	




										if (_ContinuaCriacao == true)
										{

											// cria login

											// testa erro e avisa e sai se ocorrer	



											retFunc = SD_CreateNewLoginFile(IDUserMax_Novo, 
																			gAdmin_User_CPF, 
																			gAdmin_ID_Cartao_Scan);
										


											// testa erro e avisa e sai se ocorrer
											if (retFunc.substring(0, 1) != F("1"))
											{
												gTelaRenderizada_MSGBOX = false; 
											
												TELA_Render_MsgBox(F("Erro ao cadastrar Login"), F("Ocorreu um erro ao cadastrar o novo login"));

												delay(6000);

												TELA_LimpaTela();

												gTelaRenderizada_MSGBOX = false;
												gTelaRenderizada_ADMIN_NOVO_CARD = false;

												gTelaRenderizada_TecAlfa = false;
												gTelaRenderizada_TecNum = false;	
												gTelaRenderizada_TECLADO = false;	

												_ContinuaCriacao = false;
											}	
										

										}	


										if (_ContinuaCriacao == true)
										{
				
											// exibe msg de ok e retorna para novo admin principal
											// se chegou aqui eh por que deu tudo certo

											TELA_LimpaTela();

											gTelaRenderizada_MSGBOX = false; 
											
											TELA_Render_MsgBox(F("Usuario cadastrado com sucesso !"), String(F("O usuario foi cadastrado com sucesso ! Usuario: ")) + gAdmin_User_Nome + String(F(" - Saldo: ")) + FormatNumber(gAdmin_User_Saldo, F("MONEY")) ) ;

											delay(6000);

											TELA_LimpaTela();

											gTelaRenderizada_MSGBOX = false;

											// finaliza os dados e zzera tudo
											gAdmin_ID_Cartao_Scan = F("");
											gAdmin_User_Nome = F("");
											gAdmin_User_Nivel = F("");
											gAdmin_User_CPF = F("");
											gAdmin_User_Saldo = F("");
											gAdmin_User_IDUser = F("");	
											gAdmin_User_Datacad = F("");								
		 
											gModoOperacao_SubTela = F("ADMIN_USUARIOS");

											gTelaRenderizada_ADMIN_USUARIOS = false;
											gTelaRenderizada_ADMIN_NOVO_CARD = false;

											gTelaRenderizada_TecAlfa = false;
											gTelaRenderizada_TecNum = false;
											gTelaRenderizada_TECLADO = false;

											TELA_LimpaTela();



										}


									}
									else
									{


										if (Right(_txtErro, 2) == F(", "))
										{
											_txtErro = Left(_txtErro, _txtErro.length() - 2);
										}


										TELA_LimpaTela();

										gTelaRenderizada_MSGBOX = false; 
											

										TELA_Render_MsgBox(F("Campos em branco"), _txtErro);

										delay(6000);

										TELA_LimpaTela();

										gTelaRenderizada_MSGBOX = false;
										gTelaRenderizada_ADMIN_NOVO_CARD = false;

										gTelaRenderizada_TecAlfa = false;
										gTelaRenderizada_TecNum = false;	
										gTelaRenderizada_TECLADO = false;									

									}

		

								}

						
								gTelaRenderizada_MSGBOX = false; 
											
								/*
								gModoOperacao = F("STANDBY");
								gModoOperacao_SubTela = F("");

								gTelaRenderizada_ADMIN = false;

								TELA_LimpaTela();

								delay(500);  	
								*/

								// -----------------------------------

								gBounce_time_inicio = millis();

							}


							gBounce_time_atual = millis();
							gBounce_time_tempo_passado = gBounce_time_atual - gBounce_time_inicio;

							gBounce_SegundosPassados = floor(gBounce_time_tempo_passado / 1000);

							//LogTerm(gBounce_time_tempo_passado);

							if (gBounce_SegundosPassados != gBounce_Last_SegundosPassados)
							{
								//LogTerm(time_tempo_passado);
							}



							if (gBounce_time_tempo_passado >= ctBOUNCE_SENSIB_BOTAO)
							{

								gBounce_ContaClick = 0;		

							}

							gBounce_Last_SegundosPassados = gBounce_SegundosPassados;

							// Esquema de DEBounce ---- FIM



						}

					}










				}
			}	    							 





		}

	}

}








void TELA_VerificaTouch_ADMIN_SANGRIA()
{


	if (ctTELA_HARDWARE == F("TERMINAL"))
	{ 


		String retConsole;

		retConsole = ReadConsoleInput();

		if (retConsole != F(""))
		{
			LogTerm(String(F("Opcao selecionada: ")) + retConsole);	
		}


		if ((retConsole.toInt() == 99) || (retConsole.toInt() == 98))
		{

			if (retConsole.toInt() == 99)
			{

				// zera as vars para cada tentativa de login
				// efetua logoff
				gSessao_Logado = false;
				gSessao_IDUser = -1;
				gSessao_Nome = F("");
				gSessao_Nivel = -1;
				gSessao_SaldoAtual = -1;

				gTelaRenderizada_OPERACAO = false;	

				gModoOperacao = F("STANDBY");  
				gModoOperacao_SubTela = F("");						
				
				LogTerm(F("MAIN: Usuario clicou em SAIR")); 
								
			}


			if (retConsole.toInt() == 98)
			{

				// zera as vars para cada tentativa de login
				// efetua logoff
				//todo: realizar a chamada para a rotina de admin
				gSessao_Logado = false;
				gSessao_IDUser = -1;
				gSessao_Nome = F("");
				gSessao_Nivel = -1;
				gSessao_SaldoAtual = -1;

				gTelaRenderizada_OPERACAO = false;	

				gModoOperacao = F("STANDBY");  
				gModoOperacao_SubTela = F("");						
				
				LogTerm(F("MAIN: Usuario clicou em ADMIN")); 
								
			}



		}
		else
		{
			LogTerm(String(F("TORNEIRA ")) + retConsole + String(F(" SELECIONADA")));

			gServico_ID_TorneiraAtual = retConsole.toInt();

			//gModoOperacao_SubTela = F("OPERACAO_SERVICO");


			//gModoOperacao = F("LOGIN");
			//gModoOperacao_SubTela = F("LER_RFID");			

			//gTelaRenderizada_OPERACAO = false;

			//delay(500);  				
		}



	}



	if (String(ctTELA_HARDWARE) == String(F("ER-TFTM070-5")))
	{  

		//LogTerm("gBounce_ContaClick = " + String(gBounce_ContaClick));

		//tft.changeMode(GRAPHIC);

		if (TELA_touchDetect())
		{

			TELA_touchReadPixel(&gTouch_X, &gTouch_Y);

			

			//LogTerm(String(gTouch_X) + String(" -- ") + String(gTouch_Y));

			//TELA_LogTerm_XY();	


			// botao sair


		    int btnSair_PosAtual_X = 10;
			int btnSair_PosAtual_Y = 15;

			int btnSair_Size_W = 130;
			int btnSair_Size_H = 60;			



		    int btnAdmin_PosAtual_X = 190;
			int btnAdmin_PosAtual_Y = 15;

			int btnAdmin_Size_W = 120;
			int btnAdmin_Size_H = 60;
	




			// BOTAO VOLTAR
			if ((gTouch_X >= btnSair_PosAtual_X) && (gTouch_X <= btnSair_PosAtual_X + btnSair_Size_W)) 
			{

				if ((gTouch_Y >= btnSair_PosAtual_Y) && (gTouch_Y <= btnSair_PosAtual_Y + btnSair_Size_H)) 
				{


					//LogTerm(String(F("a - gBounce_ContaClick = ")) + String(gBounce_ContaClick) + String(F(" / gBounce_time_tempo_passado = ")) + String(gBounce_time_tempo_passado) + String(F(" / gBounce_SegundosPassados = ")) + String(gBounce_SegundosPassados));

					// Esquema de DEBounce ---- inicio

					gBounce_ContaClick++;
					

					if (gBounce_ContaClick == 1)
					{
						// Local onde deve ocorrer o evento do clique. Ocorrera apenas 1 vez --------
						
						//LogTerm(String(F("1 - gBounce_ContaClick = ")) + String(gBounce_ContaClick) + String(F(" / gBounce_time_tempo_passado = ")) + String(gBounce_time_tempo_passado) + String(F(" / gBounce_SegundosPassados = ")) + String(gBounce_SegundosPassados));


						TELA_LimpaTela();

						// zera as vars para cada tentativa de login
						// efetua logoff
						//gSessao_Logado = false;
						//gSessao_IDUser = -1;
						//gSessao_Nome = F("");
						//gSessao_Nivel = -1;
						//gSessao_SaldoAtual = -1;

						gTelaRenderizada_ADMIN_SANGRIA = false;	
						 
						gModoOperacao_SubTela = F("ADMIN_OUTROS");						
						
					


						LogTerm(F("MAIN: Usuario clicou em SAIR"));
						//TELA_Texto(F("BOTAO SAIR APERTADO"), F("MAGENTA"));  
						//delay(500); 								

						//LogTerm(F("TELA_VerificaTouch_OPERACAO"));
						//TELA_LogTerm_XY();
							
						// -----------------------------------

						gBounce_time_inicio = millis();
					}


					gBounce_time_atual = millis();
					gBounce_time_tempo_passado = gBounce_time_atual - gBounce_time_inicio;

					gBounce_SegundosPassados = floor(gBounce_time_tempo_passado / 1000);

					//LogTerm(gBounce_time_tempo_passado);

					if (gBounce_SegundosPassados != gBounce_Last_SegundosPassados)
					{
						//LogTerm(time_tempo_passado);
					}



					if (gBounce_time_tempo_passado >= ctBOUNCE_SENSIB_BOTAO)
					{

						gBounce_ContaClick = 0;		

					}

					gBounce_Last_SegundosPassados = gBounce_SegundosPassados;

					// Esquema de DEBounce ---- FIM
					//LogTerm(String(F("B - gBounce_ContaClick = ")) + String(gBounce_ContaClick) + String(F(" / gBounce_time_tempo_passado = ")) + String(gBounce_time_tempo_passado) + String(F(" / gBounce_SegundosPassados = ")) + String(gBounce_SegundosPassados));


				}

			}





			//botao 1:
			if (gTouch_X >= gOffset_W && gTouch_X <= gTamBotao_W + gOffset_W)  
			{

				if (gTouch_Y >= gOffset_H && gTouch_Y <= gTamBotao_H + gOffset_H) 
				{




					// Esquema de DEBounce ---- inicio

					gBounce_ContaClick++;
					

					if (gBounce_ContaClick == 1)
					{
						// Local onde deve ocorrer o evento do clique. Ocorrera apenas 1 vez --------

						LogTerm(F("BOTAO 1 APERTADO"));
						//TELA_Texto(F("BOTAO 2 APERTADO"), F("AZUL"));
						//delay(500);
						//TELA_LogTerm_XY();    

						gServico_ID_TorneiraAtual = 1;

						//gModoOperacao_SubTela = F("OPERACAO_SERVICO");


						gModoOperacao_SubTela = F("OPERACAO_SERVICO");

						gTelaRenderizada_ADMIN_SANGRIA = false;

						TELA_LimpaTela();

						delay(500);  	
							
						// -----------------------------------

						gBounce_time_inicio = millis();

					}


					gBounce_time_atual = millis();
					gBounce_time_tempo_passado = gBounce_time_atual - gBounce_time_inicio;

					gBounce_SegundosPassados = floor(gBounce_time_tempo_passado / 1000);

					//LogTerm(gBounce_time_tempo_passado);

					if (gBounce_SegundosPassados != gBounce_Last_SegundosPassados)
					{
						//LogTerm(time_tempo_passado);
					}



					if (gBounce_time_tempo_passado >= ctBOUNCE_SENSIB_BOTAO)
					{

						gBounce_ContaClick = 0;		

					}

					gBounce_Last_SegundosPassados = gBounce_SegundosPassados;

					// Esquema de DEBounce ---- FIM



				}

			}
			else
			{
				// outros botoes

				for (int ContaBotao = 2 ; ContaBotao <= 10 ; ContaBotao++)
				{


				
					if (gTouch_X >= ContaBotao * gOffset_W + (ContaBotao - 1) * gTamBotao_W && gTouch_X <= ContaBotao * gOffset_W + ContaBotao * gTamBotao_W )  
					{

						if (gTouch_Y >= gOffset_H && gTouch_Y <= gTamBotao_H + gOffset_H) 
						{

							

							// Esquema de DEBounce ---- inicio

							gBounce_ContaClick++;
							

							if (gBounce_ContaClick == 1)
							{
								// Local onde deve ocorrer o evento do clique. Ocorrera apenas 1 vez --------

								LogTerm(String(F("TORNEIRA ")) + String(ContaBotao) + String(F(" SELECIONADA")));
								//TELA_Texto(F("BOTAO 2 APERTADO"), F("AZUL"));
								//delay(500);
								//TELA_LogTerm_XY();    

								gServico_ID_TorneiraAtual = ContaBotao;

								//gModoOperacao_SubTela = F("OPERACAO_SERVICO");

								//gModoOperacao = F("LOGIN");
								//gModoOperacao_SubTela = F("LER_RFID");

								//gTelaRenderizada_OPERACAO = false;

								gModoOperacao_SubTela = F("OPERACAO_SERVICO");

								gTelaRenderizada_ADMIN_SANGRIA = false;

								TELA_LimpaTela();

								delay(500);  	
									
								// -----------------------------------

								gBounce_time_inicio = millis();
							}


							gBounce_time_atual = millis();
							gBounce_time_tempo_passado = gBounce_time_atual - gBounce_time_inicio;

							gBounce_SegundosPassados = floor(gBounce_time_tempo_passado / 1000);

							//LogTerm(gBounce_time_tempo_passado);

							if (gBounce_SegundosPassados != gBounce_Last_SegundosPassados)
							{
								//LogTerm(time_tempo_passado);
							}



							if (gBounce_time_tempo_passado >= ctBOUNCE_SENSIB_BOTAO)
							{

								gBounce_ContaClick = 0;		

							}

							gBounce_Last_SegundosPassados = gBounce_SegundosPassados;

							// Esquema de DEBounce ---- FIM




						}

					}

				}

			}





		}

	}


}




void TELA_VerificaTouch_ADMIN_ADD_VALOR()
{

}

void TELA_VerificaTouch_ADMIN_SUB_VALOR()
{

}

void TELA_VerificaTouch_ADMIN_SET_SALDO()
{

}





void TELA_VerificaTouch_OPERACAO()
{

	//LogTerm(String(F("gTelaRenderizada_OPERACAO = ")) + String(gTelaRenderizada_OPERACAO));
	//while (1){}


	if (ctTELA_HARDWARE == F("TERMINAL"))
	{ 


		String retConsole;

		retConsole = ReadConsoleInput();

		if (retConsole != F(""))
		{
			LogTerm(String(F("Opcao selecionada: ")) + retConsole);	
		}


		if ((retConsole.toInt() == 99) || (retConsole.toInt() == 98))
		{

			if (retConsole.toInt() == 99)
			{

				// zera as vars para cada tentativa de login
				// efetua logoff
				gSessao_Logado = false;
				gSessao_IDUser = -1;
				gSessao_Nome = F("");
				gSessao_Nivel = -1;
				gSessao_SaldoAtual = -1;

				gTelaRenderizada_OPERACAO = false;	

				gModoOperacao = F("STANDBY");  
				gModoOperacao_SubTela = F("");						
				
				LogTerm(F("MAIN: Usuario clicou em SAIR")); 
								
			}


			if (retConsole.toInt() == 98)
			{

				// zera as vars para cada tentativa de login
				// efetua logoff
				//todo: realizar a chamada para a rotina de admin
				gSessao_Logado = false;
				gSessao_IDUser = -1;
				gSessao_Nome = F("");
				gSessao_Nivel = -1;
				gSessao_SaldoAtual = -1;

				gTelaRenderizada_OPERACAO = false;	

				gModoOperacao = F("STANDBY");  
				gModoOperacao_SubTela = F("");						
				
				LogTerm(F("MAIN: Usuario clicou em ADMIN")); 
								
			}



		}
		else
		{
			LogTerm(String(F("TORNEIRA ")) + retConsole + String(F(" SELECIONADA")));

			gServico_ID_TorneiraAtual = retConsole.toInt();

			//gModoOperacao_SubTela = F("OPERACAO_SERVICO");


			gModoOperacao = F("LOGIN");
			gModoOperacao_SubTela = F("LER_RFID");			

			gTelaRenderizada_OPERACAO = false;

			//delay(500);  				
		}



	}



	if (String(ctTELA_HARDWARE) == String(F("ER-TFTM070-5")))
	{  

		//LogTerm("gBounce_ContaClick = " + String(gBounce_ContaClick));

		//tft.changeMode(GRAPHIC);

		if (TELA_touchDetect())
		{

			TELA_touchReadPixel(&gTouch_X, &gTouch_Y);

			

			//LogTerm(String(gTouch_X) + String(" -- ") + String(gTouch_Y));

			//TELA_LogTerm_XY();	


			// botao sair


		    int btnSair_PosAtual_X = 10;
			int btnSair_PosAtual_Y = 15;

			int btnSair_Size_W = 130;
			int btnSair_Size_H = 60;			



		    int btnAdmin_PosAtual_X = 190;
			int btnAdmin_PosAtual_Y = 15;

			int btnAdmin_Size_W = 120;
			int btnAdmin_Size_H = 60;
	




			// BOTAO SAIR
			if ((gTouch_X >= btnSair_PosAtual_X) && (gTouch_X <= btnSair_PosAtual_X + btnSair_Size_W)) 
			{

				if ((gTouch_Y >= btnSair_PosAtual_Y) && (gTouch_Y <= btnSair_PosAtual_Y + btnSair_Size_H)) 
				{


					//LogTerm(String(F("a - gBounce_ContaClick = ")) + String(gBounce_ContaClick) + String(F(" / gBounce_time_tempo_passado = ")) + String(gBounce_time_tempo_passado) + String(F(" / gBounce_SegundosPassados = ")) + String(gBounce_SegundosPassados));

					// Esquema de DEBounce ---- inicio

					gBounce_ContaClick++;
					

					if (gBounce_ContaClick == 1)
					{
						// Local onde deve ocorrer o evento do clique. Ocorrera apenas 1 vez --------
						
						//LogTerm(String(F("1 - gBounce_ContaClick = ")) + String(gBounce_ContaClick) + String(F(" / gBounce_time_tempo_passado = ")) + String(gBounce_time_tempo_passado) + String(F(" / gBounce_SegundosPassados = ")) + String(gBounce_SegundosPassados));


						TELA_LimpaTela();

						// zera as vars para cada tentativa de login
						// efetua logoff
						gSessao_Logado = false;
						gSessao_IDUser = -1;
						gSessao_Nome = F("");
						gSessao_Nivel = -1;
						gSessao_SaldoAtual = -1;

						gTelaRenderizada_OPERACAO = false;	

						gModoOperacao = F("STANDBY");  
						gModoOperacao_SubTela = F("");						
						
					


						LogTerm(F("MAIN: Usuario clicou em SAIR"));
						//TELA_Texto(F("BOTAO SAIR APERTADO"), F("MAGENTA"));  
						//delay(500); 								

						//LogTerm(F("TELA_VerificaTouch_OPERACAO"));
						//TELA_LogTerm_XY();
							
						// -----------------------------------

						gBounce_time_inicio = millis();
					}


					gBounce_time_atual = millis();
					gBounce_time_tempo_passado = gBounce_time_atual - gBounce_time_inicio;

					gBounce_SegundosPassados = floor(gBounce_time_tempo_passado / 1000);

					//LogTerm(gBounce_time_tempo_passado);

					if (gBounce_SegundosPassados != gBounce_Last_SegundosPassados)
					{
						//LogTerm(time_tempo_passado);
					}



					if (gBounce_time_tempo_passado >= ctBOUNCE_SENSIB_BOTAO)
					{

						gBounce_ContaClick = 0;		

					}

					gBounce_Last_SegundosPassados = gBounce_SegundosPassados;

					// Esquema de DEBounce ---- FIM
					//LogTerm(String(F("B - gBounce_ContaClick = ")) + String(gBounce_ContaClick) + String(F(" / gBounce_time_tempo_passado = ")) + String(gBounce_time_tempo_passado) + String(F(" / gBounce_SegundosPassados = ")) + String(gBounce_SegundosPassados));


				}

			}




			// BOTAO ADMIN
			if ((gTouch_X >= btnAdmin_PosAtual_X) && (gTouch_X <= btnAdmin_PosAtual_X + btnAdmin_Size_W)) 
			{

				if ((gTouch_Y >= btnAdmin_PosAtual_Y) && (gTouch_Y <= btnAdmin_PosAtual_Y + btnAdmin_Size_H)) 
				{


					//LogTerm(String(F("a - gBounce_ContaClick = ")) + String(gBounce_ContaClick) + String(F(" / gBounce_time_tempo_passado = ")) + String(gBounce_time_tempo_passado) + String(F(" / gBounce_SegundosPassados = ")) + String(gBounce_SegundosPassados));

					// Esquema de DEBounce ---- inicio

					gBounce_ContaClick++;
					

					if (gBounce_ContaClick == 1)
					{
						// Local onde deve ocorrer o evento do clique. Ocorrera apenas 1 vez --------
						
						//LogTerm(String(F("1 - gBounce_ContaClick = ")) + String(gBounce_ContaClick) + String(F(" / gBounce_time_tempo_passado = ")) + String(gBounce_time_tempo_passado) + String(F(" / gBounce_SegundosPassados = ")) + String(gBounce_SegundosPassados));


						LogTerm(F("BOTAO ADMIN APERTADO"));

						//gModoOperacao_SubTela = F("OPERACAO_SERVICO");


						gModoOperacao = F("LOGIN");
						gModoOperacao_SubTela = F("LER_RFID_ADMIN");

						gTelaRenderizada_OPERACAO = false;
						//murch
						gTelaRenderizada_ADMIN = false;

						TELA_LimpaTela();

						//delay(500);  	


						/*
						// procedimento para sair para tela inicial

						TELA_LimpaTela();

						// zera as vars para cada tentativa de login
						// efetua logoff
						gSessao_Logado = false;
						gSessao_IDUser = -1;
						gSessao_Nome = F("");
						gSessao_Nivel = -1;
						gSessao_SaldoAtual = -1;

						gTelaRenderizada_OPERACAO = false;	

						gModoOperacao = F("ADMIN");  
						gModoOperacao_SubTela = F("");						
						
					

						*/

						LogTerm(F("MAIN: Usuario clicou em ADMIN"));


							
						// -----------------------------------

						gBounce_time_inicio = millis();
					}


					gBounce_time_atual = millis();
					gBounce_time_tempo_passado = gBounce_time_atual - gBounce_time_inicio;

					gBounce_SegundosPassados = floor(gBounce_time_tempo_passado / 1000);

					//LogTerm(gBounce_time_tempo_passado);

					if (gBounce_SegundosPassados != gBounce_Last_SegundosPassados)
					{
						//LogTerm(time_tempo_passado);
					}



					if (gBounce_time_tempo_passado >= ctBOUNCE_SENSIB_BOTAO)
					{

						gBounce_ContaClick = 0;		

					}

					gBounce_Last_SegundosPassados = gBounce_SegundosPassados;

					// Esquema de DEBounce ---- FIM
					//LogTerm(String(F("B - gBounce_ContaClick = ")) + String(gBounce_ContaClick) + String(F(" / gBounce_time_tempo_passado = ")) + String(gBounce_time_tempo_passado) + String(F(" / gBounce_SegundosPassados = ")) + String(gBounce_SegundosPassados));


				}

			}


			//botao 1:
			if (gTouch_X >= gOffset_W && gTouch_X <= gTamBotao_W + gOffset_W)  
			{

				if (gTouch_Y >= gOffset_H && gTouch_Y <= gTamBotao_H + gOffset_H) 
				{




					// Esquema de DEBounce ---- inicio

					gBounce_ContaClick++;
					

					if (gBounce_ContaClick == 1)
					{
						// Local onde deve ocorrer o evento do clique. Ocorrera apenas 1 vez --------

						LogTerm(F("BOTAO 1 APERTADO"));
						//TELA_Texto(F("BOTAO 2 APERTADO"), F("AZUL"));
						//delay(500);
						//TELA_LogTerm_XY();    

						gServico_ID_TorneiraAtual = 1;

						//gModoOperacao_SubTela = F("OPERACAO_SERVICO");


						gModoOperacao = F("LOGIN");
						gModoOperacao_SubTela = F("LER_RFID");

						gTelaRenderizada_OPERACAO = false;

						TELA_LimpaTela();

						delay(500);  	
							
						// -----------------------------------

						gBounce_time_inicio = millis();

					}


					gBounce_time_atual = millis();
					gBounce_time_tempo_passado = gBounce_time_atual - gBounce_time_inicio;

					gBounce_SegundosPassados = floor(gBounce_time_tempo_passado / 1000);

					//LogTerm(gBounce_time_tempo_passado);

					if (gBounce_SegundosPassados != gBounce_Last_SegundosPassados)
					{
						//LogTerm(time_tempo_passado);
					}



					if (gBounce_time_tempo_passado >= ctBOUNCE_SENSIB_BOTAO)
					{

						gBounce_ContaClick = 0;		

					}

					gBounce_Last_SegundosPassados = gBounce_SegundosPassados;

					// Esquema de DEBounce ---- FIM



				}

			}
			else
			{
				// outros botoes

				for (int ContaBotao = 2 ; ContaBotao <= 10 ; ContaBotao++)
				{


				
					if (gTouch_X >= ContaBotao * gOffset_W + (ContaBotao - 1) * gTamBotao_W && gTouch_X <= ContaBotao * gOffset_W + ContaBotao * gTamBotao_W )  
					{

						if (gTouch_Y >= gOffset_H && gTouch_Y <= gTamBotao_H + gOffset_H) 
						{

							

							// Esquema de DEBounce ---- inicio

							gBounce_ContaClick++;
							

							if (gBounce_ContaClick == 1)
							{
								// Local onde deve ocorrer o evento do clique. Ocorrera apenas 1 vez --------

								LogTerm(String(F("TORNEIRA ")) + String(ContaBotao) + String(F(" SELECIONADA")));
								//TELA_Texto(F("BOTAO 2 APERTADO"), F("AZUL"));
								//delay(500);
								//TELA_LogTerm_XY();    

								gServico_ID_TorneiraAtual = ContaBotao;

								//gModoOperacao_SubTela = F("OPERACAO_SERVICO");

								gModoOperacao = F("LOGIN");
								gModoOperacao_SubTela = F("LER_RFID");

								gTelaRenderizada_OPERACAO = false;

								TELA_LimpaTela();

								delay(500);  	
									
								// -----------------------------------

								gBounce_time_inicio = millis();
							}


							gBounce_time_atual = millis();
							gBounce_time_tempo_passado = gBounce_time_atual - gBounce_time_inicio;

							gBounce_SegundosPassados = floor(gBounce_time_tempo_passado / 1000);

							//LogTerm(gBounce_time_tempo_passado);

							if (gBounce_SegundosPassados != gBounce_Last_SegundosPassados)
							{
								//LogTerm(time_tempo_passado);
							}



							if (gBounce_time_tempo_passado >= ctBOUNCE_SENSIB_BOTAO)
							{

								gBounce_ContaClick = 0;		

							}

							gBounce_Last_SegundosPassados = gBounce_SegundosPassados;

							// Esquema de DEBounce ---- FIM




								

								





						}

					}

				}

			}





		}

	}

}






void TELA_VerificaTouch_DEBUG()
{

	//TELA_VerificaTouch_TECLADO_NUM();
	//TELA_VerificaTouch_TECLADO_ALFA();

}







