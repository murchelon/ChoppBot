/*
  ChoppBot 1.0

  By Marcelo Rocha, Sidnei Conrad and Flavio Liberal

  Main program to an automated chopp dispenser with several
  features like touchscreen, biometrical recognition and RFID
  recognition. More to came....
  teste chopp 
*/



#include "BIB/Main/ChoppBot_Main_BIB.h" 






///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  PROCEDURES ChoopBot
//  ===================
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// Inicia a aplicacao, definido todos os parametros necessarios
void InitApp()
{




    Serial.begin(115200);       // 115200 pois o RFID precisa desta velocidade, para acompanharmos o que ele escreve no serial


    // Wait for USB Serial 
    while (!Serial) 
    {
        SysCall::yield();
    }



    InicializaVars();



    LogTerm(F("/////////////////////////////////////////////////////"));
    LogTerm(F("///                                               ///"));
    LogTerm(String(F("///  ChoppBot ")) + String(VersaoAPP) + String(F("                                 ///")));
    LogTerm(F("///  ============                                 ///"));
    LogTerm(F("///                                               ///"));
    LogTerm(F("///  by Murch & Cebola                            ///"));
    LogTerm(F("///                                               ///"));
    LogTerm(F("/////////////////////////////////////////////////////"));
    LogTerm(F(""));
    LogTerm(F("MAIN: Iniciando sistema..."));


    
    LED_SetLedState("BUILTIN", false, "");


    // da uma piscada no led interno para demonstrar inicio
    for (int x = 0 ; x <= 10 ; x++)
    {
        LED_SetLedState("BUILTIN", false, "");
        delay(40);
        LED_SetLedState("BUILTIN", true, "");
        delay(40);
        //LogTerm(F("ok to aqui"));
    }

    LED_SetLedState("BUILTIN", false, "");



    // Inicia o LED do rfid
    pinMode(ctPINO_LED_CORFIXA, OUTPUT);


    // inicia o led interno arduino, usado para mostrar que o programa esta rodando
    pinMode(LED_BUILTIN, OUTPUT);

    // Inicia o BUZZER
    pinMode(BUZZER_PINO, OUTPUT);



    // Inicia os RELES
    pinMode(RELE_PINO_TORNEIRA_1, OUTPUT);
    pinMode(RELE_PINO_TORNEIRA_2, OUTPUT);
    pinMode(RELE_PINO_TORNEIRA_3, OUTPUT);

    // Define os RELES como DESLIGADOS
    digitalWrite(RELE_PINO_TORNEIRA_1, HIGH);
    digitalWrite(RELE_PINO_TORNEIRA_2, HIGH);
    digitalWrite(RELE_PINO_TORNEIRA_3, HIGH);




    // Inicia os FLOW METERS
    pinMode(FLOW_PINO_SENSOR_1, INPUT);
    digitalWrite(FLOW_PINO_SENSOR_1, HIGH);


    pinMode(FLOW_PINO_SENSOR_2, INPUT);
    digitalWrite(FLOW_PINO_SENSOR_2, HIGH);


    pinMode(FLOW_PINO_SENSOR_3, INPUT);
    digitalWrite(FLOW_PINO_SENSOR_3, HIGH);


    digitalWrite(SD_MAX_TENTATIVA_READ, HIGH);
    digitalWrite(SD_DELAY_TENTATIVA_READ, HIGH);
    digitalWrite(SD_PINO, HIGH);


    digitalWrite(RFID_PINO_SCK, HIGH);
    digitalWrite(RFID_PINO_MOSI, HIGH);
    digitalWrite(RFID_PINO_SS, HIGH);
    digitalWrite(RFID_PINO_MISO, HIGH);
    digitalWrite(RFID_PINO_IRQ, HIGH);
    digitalWrite(RFID_PINO_RESET, HIGH);

    delay(500);

    TELA_IniciaTela();

    TELA_Texto(String(F("Choppbot ")) + String(VersaoAPP), F("VERDE"));
    TELA_Texto(F("============"), F("VERDE"));
    TELA_Texto(F(""), F("BRANCO"));
    TELA_Texto(F("Iniciando sistema..."), F("BRANCO"));
    TELA_Texto(F(""), F("BRANCO"));



    



    LogTerm(F("====  Engates  ===="));

    String retEngatados = "";

    retEngatados = BANCO_DefineChoppEngatados(gaEngatados);  

    if (retEngatados.substring(0, 1) != "1") 
    {

        BUZZER_TocaSom(F("ERRO"));


        LogTerm(F("MAIN: Falha ao carregar arquivo com os chopps engatados"));
        LogTerm(String(F("MAIN: Erro: ")) + retEngatados.substring(3));
        LogTerm(F("MAIN: Fallha critica. O sistema sera reiniciado em 10 segundos..."));

        if (String(ctTELA_HARDWARE) == String(F("ER-TFTM070-5")))
        {       
            TELA_Texto(F("MAIN: Falha ao carregar arquivo com os chopps engatados"), F("BRANCO"));
            TELA_Texto(String(F("MAIN: Erro: ")) + retEngatados.substring(3), F("AMARELO"));
            TELA_Texto(F("MAIN: Fallha critica. O sistema sera reiniciado em 10 segundos..."), F("BRANCO"));
        }


        delay(10000);
        resetFunc();        
    }

    BUZZER_TocaSom(F("LIGAR"));


    // NumTorneira;DataCad;IDChopp;VolumeAtual;DataExpira;Ativa;NomeFromBanco
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

            //LogTerm(gaEngatados[x]);

            LogTerm(String(F("Torneira [")) + String(x) + String(F("] -- IDChopp: ")) + tmp_IDChopp);
            LogTerm(String(F("Torneira [")) + String(x) + String(F("] -- Nome: ")) + tmp_Nome);
            LogTerm(String(F("Torneira [")) + String(x) + String(F("] -- Tipo: ")) + tmp_Tipo);
            LogTerm(String(F("Torneira [")) + String(x) + String(F("] -- Valor: ")) + tmp_Valor);
            LogTerm(String(F("Torneira [")) + String(x) + String(F("] -- Volume Atual: ")) + tmp_Volume);
            //LogTerm(F("Torneira [" + String(x) + "] -- Data de Cadastro: " + tmp_DataCad);
            //LogTerm(F("Torneira [" + String(x) + "] -- Data de Expiracao: " + tmp_DataExp);

            if (tmp_Ativa == F("1"))
            {
                LogTerm(String(F("Torneira [")) + String(x) + String(F("] -- Ativa: SIM")));
            }
            else
            {
                LogTerm(String(F("Torneira [")) + String(x) + String(F("] -- Ativa: NAO"))); 
            }
            
            LogTerm(F("---------"));

        }


    }

    

    //BIOMETRICO_Inicia();




    // todo: inicializa hardware, fecha valvulas, etc

    LogTerm(F("MAIN: Sistema Inicializado."));



    if (ctTELA_HARDWARE == String(F("ER-TFTM070-5")))
    {
        TELA_LimpaTela();
    }

    

}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  EXEC_INICIO
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    
// ROTINA DE INICIO
void Exec_INICIO()
{

    LogTerm(F("== [Modo Atual: INICIO] =="));

    
    gModoOperacao = F("STANDBY");



    //TELA_Render_MsgBox("", "X");
    //TELA_Render_MsgBox("Meu nome e MARCELO wef webfwef dd d dX");
    //TELA_Render_MsgBox("1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456");
    //TELA_Render_MsgBox("12345678901234567890!@#$%^&*()abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");

    //TELA_Render_MsgBox("Ola! Meu nome e MARCELO ROCHA. Eu estou verificando o tamanho desta caixa de msgbox para que seja possivel escerever um texto demasiadamente grande sem problemas. Obrigado.");
    //TELA_Render_MsgBox("Super teste de Janela", "Ola! Meu nome e MARCELO ROCHA.^Eu estou verificando o tamanho desta caixa de msgbox^para que seja possivel escerever um texto demasiadamente grande sem problemas. Obrigado.^^Teste^opa");
    //TELA_Render_MsgBox("Teste de Enter", "Teste de oula linha:^Linha2");

}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  EXEC_INSTALACAO
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

   

// ROTINA DE INSTALACAO
void Exec_INSTALACAO()
{

    if (gTelaRenderizada_INSTALACAO == false)
    {
        LogTerm(F("== [Modo Atual: INSTALACAO] =="));
    }  

    boolean Exec_Loop_PodeSair = false;

    unsigned long time_inicio;
    unsigned long time_atual;
    unsigned long time_tempo_passado;

    int SegundosPassados;
    int Last_SegundosPassados;

    //gModoOperacao = "INSTALACAO";

    time_inicio = millis();

    while (Exec_Loop_PodeSair == false)
    {

        time_atual = millis();
        time_tempo_passado = time_atual - time_inicio;

        SegundosPassados = floor(time_tempo_passado / 1000);

        if (SegundosPassados != Last_SegundosPassados)
        {
            LogTerm(SegundosPassados);
        }



        if (time_tempo_passado >= gTimeoutOpcao)
        {
            Exec_Loop_PodeSair = true;
        }

        Last_SegundosPassados = SegundosPassados;

        //delay(500);

    }
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  EXEC_LOGIN
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

   


// ROTINA DE LOGIN
void Exec_LOGIN()
{
    if (gTelaRenderizada_LOGIN == false)
    {
        LogTerm(F("== [Modo Atual: LOGIN] =="));
    }  

    TELA_Render_Interface_LOGIN();

}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  EXEC_OPERACAO
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

   


// ROTINA DE OPERACAO
void Exec_OPERACAO()
{
    if (gTelaRenderizada_OPERACAO == false)
    {
        LogTerm(F("== [Modo Atual: OPERACAO] =="));
    }  

    TELA_Render_Interface_OPERACAO();

}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  EXEC_ADMIN
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

   



// ROTINA DE ADMIN
void Exec_ADMIN()
{
    boolean Exec_Loop_PodeSair = false;

    unsigned long time_inicio;
    unsigned long time_atual;
    unsigned long time_tempo_passado;

    int SegundosPassados;
    int Last_SegundosPassados;

    if (gTelaRenderizada_ADMIN == false)
    {
        LogTerm(F("== [Modo Atual: ADMIN] =="));
    }  

    gTelaRenderizada_ADMIN = true;


    time_inicio = millis();

    TELA_Texto(String(F("MODO ADMIN: Saindo em ")) + String(gTimeoutOpcao / 1000) + String(F(" segundos...")), F("BRANCO"));

    while (Exec_Loop_PodeSair == false)
    {


        time_atual = millis();
        time_tempo_passado = time_atual - time_inicio;

        SegundosPassados = floor(time_tempo_passado / 1000);



        if (SegundosPassados != Last_SegundosPassados)
        {
            //LogTerm(SegundosPassados);
            TELA_Texto(String(SegundosPassados), F("AMARELO"));
        }



        if (time_tempo_passado >= gTimeoutOpcao)
        {
            Exec_Loop_PodeSair = true;
        }

        Last_SegundosPassados = SegundosPassados;

        //delay(500);
    }


    if (ctTELA_HARDWARE == String(F("ER-TFTM070-5")))
    {
        TELA_LimpaTela();
    }

    
    gTelaRenderizada_ADMIN = false;
    gModoOperacao = F("STANDBY");


}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  EXEC_DEBUG
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

   



// ROTINA DE DEBUG
void Exec_DEBUG()
{
    if (gTelaRenderizada_DEBUG == false)
    {
        LogTerm(F("== [Modo Atual: DEBUG] =="));
    }  

    gTelaRenderizada_DEBUG = true;

    //TELA_RenderTecUnificado_NUM();
    TELA_RenderTecUnificado_ALFA();

}


    
    

    

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  EXEC_TESTE
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

   






// Rotina que executa o procedimento de TESTE
void Exec_TESTE()
{

    if (gTelaRenderizada_TESTE == false)
    {
        LogTerm(F("== [Modo Atual: TESTE] =="));
    }  

    boolean Exec_Loop_PodeSair = false;

    LogTerm(F("Exec_TESTE"));

    //ShowExample_Tela();

    // TELA_IniciaTela();

    // TELA_Texto("Linha1cccc");
    // TELA_Texto("Linha2");
    // TELA_Texto("Linha3");
    // TELA_Texto("Linha4");
    // TELA_Texto("Linha5");
    // TELA_Texto("Linha6");
    // TELA_Texto("Linha7");



    while (Exec_Loop_PodeSair == false)
    {
        LogTerm(F("Exec_TESTE"));
        delay(500);

    }
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  EXEC_STANDBY
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

   

// ROTINA DE STANDBY
void Exec_STANDBY()
{
    if (gTelaRenderizada_STANDBY == false)
    {
        LogTerm(F("== [Modo Atual: STANDBY] =="));
    }    
    

    TELA_Render_Interface_STANDBY();

    

}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  ROTINAS SECUNDARIAS (Rotinas como tela para edicao de dados, ler rfid, etc)
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

   


void Exec_LER_RFID()
{
    if (gTelaRenderizada_LER_RFID == false)
    {
        LogTerm(F("== [Modo Atual: LOGIN -- SubTela: LER_RFID] =="));
        TELA_Render_Interface_LER_RFID();
    }  

    

}


void Exec_OPERACAO_SERVICO()
{
    

    if (gTelaRenderizada_OPERACAO_SERVICO == false)
    {
        LogTerm(F("== [Modo Atual: OPERACAO -- SubTela: OPERACAO_SERVICO] =="));
        TELA_Render_Interface_OPERACAO_SERVICO();
    }  

    

}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




// Procedure que testa se ocorreu algum interrupt: O sistema fica em modo StandBy ate que alguem toque na tela, ou faca alguma outra acao que gere uma interrupcao
void TestaInterrupts()
{



    if (gModoOperacao == F("INICIO"))
    {

    }

    if (gModoOperacao == F("INSTALACAO"))
    {

    }

    if (gModoOperacao == F("STANDBY"))
    {
                
        TELA_VerificaTouch_STANDBY();
        
    }

    if (gModoOperacao == F("LOGIN"))
    {

        if (gModoOperacao_SubTela == F(""))
        {
            TELA_VerificaTouch_LOGIN();
        }

        if (gModoOperacao_SubTela == F("LER_RFID"))
        {
            TELA_VerificaTouch_LER_RFID();
        }

        
    }

    if (gModoOperacao == F("OPERACAO"))
    {



        if (gModoOperacao_SubTela == F(""))
        {
            TELA_VerificaTouch_OPERACAO();
        }

        if (gModoOperacao_SubTela == F("OPERACAO_SERVICO"))
        {
            TELA_VerificaTouch_OPERACAO_SERVICO();
        }

        
    }

    if (gModoOperacao == F("ADMIN"))
    {
        TELA_VerificaTouch_ADMIN();
    }

    if (gModoOperacao == F("DEBUG"))
    {
        TELA_VerificaTouch_DEBUG();
    }

    if (gModoOperacao == F("TESTE"))
    {

    }



}






///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  INTERNAS ARDUINO
//  ================
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



// INTERNA ARDUINO: SETUP
void setup()
{  
    InitApp();  
}


// INTERNA ARDUINO: LOOP ETERNO
void loop() 
{

    
    //Led_Light(false);

    delay(50);

    //debug:
    //delay(1000);

    ExecLedON_Beat();

    //Led_Light(true);
    //LogTerm(F("estou no loop");




    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    if (gModoOperacao == F("INICIO"))
    {
        Exec_INICIO();       
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    if (gModoOperacao == F("STANDBY"))
    {
        Exec_STANDBY();
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    if (gModoOperacao == F("LOGIN"))
    {

        if (gModoOperacao_SubTela == F(""))
        {
            Exec_LOGIN();  
        }
        
       if (gModoOperacao_SubTela == F("LER_RFID"))
        {
            Exec_LER_RFID();  
        }        

    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    if (gModoOperacao == F("OPERACAO"))
    {

        if (gModoOperacao_SubTela == F(""))
        {
            Exec_OPERACAO();  
        }
        
       if (gModoOperacao_SubTela == F("OPERACAO_SERVICO"))
        {
            Exec_OPERACAO_SERVICO();  
        }        

    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    if (gModoOperacao == F("ADMIN"))
    {
        Exec_ADMIN();
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    if (gModoOperacao == F("DEBUG"))
    {
        Exec_DEBUG();
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////


    if (gModoOperacao == F("INSTALACAO"))
    {
        LogTerm(F("Modo: INSTALACAO"));

        Exec_INSTALACAO();

        LogTerm(F("Modo: == StandBy =="));
        gModoOperacao = F("STANDBY");

    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    TestaInterrupts();

}

