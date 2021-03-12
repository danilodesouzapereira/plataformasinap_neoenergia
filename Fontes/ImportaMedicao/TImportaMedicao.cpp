// arquivos inclu�dos ---------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TImportaMedicao.h"
#include "VTMedicao.h"
#include "VTMedicoes.h"
#include "..\Apl\VTApl.h"
#include "..\Complexo\Complexo.h"
#include "..\Constante\Const.h"
#include "..\Constante\Fases.h"
#include "..\Curvas\VTCurva.h"
#include "..\Diretorio\VTPath.h"
#include "..\Flow\VTFlow.h"
#include "..\Log\VTLog.h"
#include "..\Patamar\VTPatamar.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Radial\VTSubestacao.h"
#include "..\Radial\VTPrimario.h"
#include "..\Radial\VTRadial.h"
#include "..\Rede\VTCanal.h"
#include "..\Rede\VTChave.h"
#include "..\Rede\VTFases.h"
#include "..\Rede\VTLigacao.h"
#include "..\Rede\VTMedidor.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTResflowlig.h"
#include "..\Rede\VTTipoChave.h"
#include "..\..\DLL_Inc\Curvas.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Log.h"

// ---------------------------------------------------------------------------
VTImportaMedicao* __fastcall NewObjImportaMedicao(TComponent *Owner, VTApl *apl_owner)
{
    try
    {
        return (new TImportaMedicao(Owner, apl_owner));
    }
    catch (Exception &e)
    {
    }
    // erro na cria��o do objeto
    return (NULL);
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/18381986
__fastcall TImportaMedicao::TImportaMedicao(TComponent *Owner, VTApl *apl_owner)
{
    // salva ponteiro p/ objetos
    this->apl_owner = apl_owner;
    // assume log habilitado
    LogEnabled = true;
    // cria objetos
    lines = new TStringList();
    campos = new TStringList();
    hm = new TStringList();
    lisREDE_VAL = new TList();
    // inicia vari�veis
    tipo_canal = canalINDEF;
    por_fase = false;
    val_max = false;
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/18381986
__fastcall TImportaMedicao::~TImportaMedicao(void)
{
    // destr�i objetos
    if (log)
    {
        delete log;
        log = NULL;
    }
    if (lines)
    {
        delete lines;
        lines = NULL;
    }
    if (campos)
    {
        delete campos;
        campos = NULL;
    }
    if (hm)
    {
        delete hm;
        hm = NULL;
    }
    if (lisREDE_VAL)
    {
        delete lisREDE_VAL;
        lisREDE_VAL = NULL;
    }
}

/*
 //---------------------------------------------------------------------------
 AnsiString __fastcall TImportaMedicao::ExisteDisjuntor(AnsiString cod_pri)
 {//DVK 2015.07.20
 //variaveis locais
 AnsiString  cod_dj = "";
 VTChave    *chave;
 VTLigacao  *ligacao;
 VTPrimario *primario;
 VTRadial   *radial = (VTRadial*)apl_owner->GetObject(__classid(VTRadial));
 VTRede     *rede;
 VTRedes    *redes = (VTRedes*)apl_owner->GetObject(__classid(VTRedes));
 TList      *lisBAR, *lisLIG;

 //prote��o
 if (redes == NULL)         return(cod_dj);
 if (radial == NULL)        return(cod_dj);
 //localiza primario
 if ((rede     = redes->ExisteRede(cod_pri))   == NULL) return(cod_dj);
 if ((primario = radial->ExistePrimario(rede)) == NULL) return(cod_dj);
 if (! rede->Carregada)                                 return(cod_dj);
 if (primario->LigacaoInicial == NULL)                  return(cod_dj);
 //verifica se a liga��o inicial � uma chave
 if (primario->LigacaoInicial->Tipo() == eqptoCHAVE)
 {
 chave = (VTChave*)primario->LigacaoInicial;
 return(chave->Codigo);
 }
 //cria listas
 lisBAR = new TList();
 lisLIG = new TList();
 //ordena liga��es
 primario->Ordena(lisBAR, lisLIG);
 for(int nc = 0; nc < lisLIG->Count; nc++)
 {
 ligacao = (VTLigacao*)lisLIG->Items[nc];
 if(ligacao->Tipo() == eqptoCHAVE)
 {//localiza primeira chave
 chave = (VTChave*)ligacao;
 break;
 }
 }
 //destroi listas
 delete lisBAR;
 delete lisLIG;
 //retorna chave localizada
 return(chave->Codigo);
 }
 */
// ---------------------------------------------------------------------------
void __fastcall TImportaMedicao::CompletaDadosCanal(VTCanal *canal)
{
    // vari�veis locais
    VTChave *chave;

    // insere demais dados do canal novo
    canal->Codigo = canal->TipoCanalAsString;
    canal->Enabled = true;
    canal->Curva = DLL_NewObjCurva();
    canal->Curva->Tipica = false;
    canal->Curva->Escala = pat01HORA;
    canal->Curva->NumeroValores = nvCURVA_CANAL;
    canal->Curva->Reconfigura(pat01HORA);
    // verifica se Canal est� associada a uma Chave
    if (canal->Eqpto->Tipo() == eqptoCHAVE)
    { // verifica se a Chave � um Disjuntor
        chave = (VTChave*)canal->Eqpto;
        if (chave->TipoChave->Tipo == chaveDISJUNTOR)
            canal->Tolerancia_perc = TOLERANCIA_MEDICAO_DJ;
        else
            canal->Tolerancia_perc = TOLERANCIA_MEDICAO_CHV;
    }
    switch (canal->TipoCanal)
    {
    case canalV:
        canal->Curva->Unidade = unidKV;
        break;
    case canalI:
        canal->Curva->Unidade = unidA;
        break;
    case canalP:
        canal->Curva->Unidade = unidKW;
        break;
    case canalQ:
        canal->Curva->Unidade = unidKVAR;
        break;
    case canalS:
        canal->Curva->Unidade = unidKVA;
        break;
    default:
        break;
    }
}

// ---------------------------------------------------------------------------
void __fastcall TImportaMedicao::ConverteUnidade(double valor[MAX_FASE])
{
    // verifica o tipo do canal
    if (((tipo_canal == canalP) || (tipo_canal == canalQ)) || (tipo_canal == canalS))
    {
        valor[indFASE_A] /= 1000.; // kW => MW  ou kvar => Mvar ou kVA => MVA
        valor[indFASE_B] /= 1000.; // kW => MW  ou kvar => Mvar ou kVA => MVA
        valor[indFASE_C] /= 1000.; // kW => MW  ou kvar => Mvar ou kVA => MVA
    }
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/18381986
VTChave* __fastcall TImportaMedicao::ExisteChave(AnsiString cod_pri, AnsiString cod_chave)
{
    // variaveis locais
    VTChave *chave;
    VTPrimario *primario;
    VTRedes *redes = (VTRedes*)apl_owner->GetObject(__classid(VTRedes));

    if (cod_pri.IsEmpty())
    { // localiza chave em qualquer rede
        if (cod_chave.IsEmpty())
            return (NULL);
        chave = (VTChave*)redes->ExisteEqpto(eqptoCHAVE, cod_chave);
    }
    else
    { // verifica se existe o Primario
        if ((primario = ExistePrimario(cod_pri)) == NULL)
            return (NULL);
        // verifica se a rede faz parte da lista de redes validas
        if (lisREDE_VAL->IndexOf(primario->Rede) < 0)
            return (NULL);
        // se o c�digo da Chave n�o est� definido, assume que � chave inicial
        if (cod_chave.IsEmpty())
            chave = ExisteChaveInicial(primario);
        else
            chave = (VTChave*)primario->Rede->ExisteLigacao(eqptoCHAVE, cod_chave);
    }
    return (chave);
}

// ---------------------------------------------------------------------------
VTChave* __fastcall TImportaMedicao::ExisteChaveInicial(VTPrimario *primario)
{
    // variaveis locais
    VTChave *chave;
    VTLigacao *ligacao;

    // verifica se a Liga��o inicial do Prim�rio � uma chave
    if (primario->LigacaoInicial == NULL)
        return (NULL);
    // verifica se a liga��o inicial � uma chave
    if (primario->LigacaoInicial->Tipo() == eqptoCHAVE)
    { //
        chave = (VTChave*)primario->LigacaoInicial;
        if (chave->Fechada)
            return (chave);
    }
    return (NULL);
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/18630318
VTPrimario* __fastcall TImportaMedicao::ExistePrimarioCodigoRede(AnsiString cod_rede_medicao)
{
    // vari�veis locais
    AnsiString  cod_rede_filtrado;
    VTPrimario  *primario;
    TList       *lisPRIMARIO = radial->LisPrimario();

    // filtra c�digo da rede da medi��o //DVK 2015.10.05
    cod_rede_medicao = FiltraCodigoRede(cod_rede_medicao);
    // verifica se existe um Prim�rio cujo c�digo filtrado � igual ao codigo da rede definido na medicao
    for (int n = 0; n < lisPRIMARIO->Count; n++)
    {
        primario = (VTPrimario*)lisPRIMARIO->Items[n];
        // filtra c�digo da Rede do Prim�rio
        cod_rede_filtrado = FiltraCodigoRede(primario->Rede->Codigo);
        if (cod_rede_filtrado.AnsiCompareIC(cod_rede_medicao) == 0)
            return (primario);
    }
    return (NULL);
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/18381986
VTPrimario* __fastcall TImportaMedicao::ExistePrimario(AnsiString cod_rede_medicao)
{
    // vari�veis locais
    AnsiString cod_rede_filtrado;
	VTPrimario *primario;
	TList *lisPRIMARIO = radial->LisPrimario();

    // filtra c�digo da rede da medi��o //DVK 2015.10.05
    cod_rede_medicao = FiltraCodigoRede(cod_rede_medicao);
    // verifica se existe um Prim�rio cujo c�digo filtrado � igual ao codigo da rede definido na medicao
	for (int n = 0; n < lisPRIMARIO->Count; n++)
	{
		primario = (VTPrimario*)lisPRIMARIO->Items[n];
        // verifica se a rede faz parte da lista de redes validas
        if (lisREDE_VAL->IndexOf(primario->Rede) < 0)
            continue;
		// filtra c�digo da Rede do Prim�rio
		cod_rede_filtrado = FiltraCodigoRede(primario->Rede->Codigo);
        if (cod_rede_filtrado.AnsiCompareIC(cod_rede_medicao) == 0)
			return (primario);
    }
    return (NULL);
}

// ---------------------------------------------------------------------------
VTSubestacao* __fastcall TImportaMedicao::ExisteSubestacao(AnsiString cod_sub_medicao)
{
    // vari�veis locais
    AnsiString cod_rede_filtrado;
	VTSubestacao *subestacao;
    TList *lisSUBS= radial->LisSubestacao();

    // filtra c�digo da rede da medi��o //DVK 2015.10.05
	cod_sub_medicao = FiltraCodigoRede(cod_sub_medicao);
	// verifica se existe um Prim�rio cujo c�digo filtrado � igual ao codigo da rede definido na medicao
	for (int n = 0; n < lisSUBS->Count; n++)
    {
		subestacao = (VTSubestacao*)lisSUBS->Items[n];
		if (!subestacao->Rede->Carregada)
            continue;
		// filtra c�digo da Rede do Prim�rio
		cod_rede_filtrado = FiltraCodigoRede(subestacao->Rede->Codigo);
		if (cod_rede_filtrado.AnsiCompareIC(cod_sub_medicao) == 0)
			return (subestacao);
    }
	return (NULL);
}

// ---------------------------------------------------------------------------
int __fastcall TImportaMedicao::ExtraiCampos(AnsiString linha, AnsiString separadores)
{
    // reinicia campos e extrai os campos do string
    campos->Clear();
    ExtraiStrings(linha.Trim(), separadores, campos);
    return (campos->Count);
}

/*
 //---------------------------------------------------------------------------
 AnsiString __fastcall TImportaMedicao::FiltraCodigoRede(AnsiString cod_orig)
 {
 //vari�veis locais
 bool       num_dig = 0;
 char       caractere;
 AnsiString cod_dest;
 int        len = cod_orig.Length();

 //filtra c�digo, deixando apenas letras e d�gitos. Descarta o primeiro d�gito, se for zero
 for (int ind = 1; ind <= len; ind++)
 {
 caractere = cod_orig[ind];
 if      ((caractere >= 'A')&&(caractere <= 'Z')) cod_dest += caractere;
 else if ((caractere >= 'a')&&(caractere <= 'z')) cod_dest += caractere;
 else if ((caractere >= '0')&&(caractere <= '9'))
 {//incrementa n�mero de d�gitos do c�digo original
 num_dig++;
 //se o primeiro d�gito for zero, n�o inclui n� c�digo final
 if ((num_dig == 1)&&(caractere == '0'))
 {//N�O inclui d�gito '0'
 //cod_dest += caractere;
 }
 else
 {//inclui o d�gito
 cod_dest += caractere;
 }
 }
 }
 return(cod_dest);
 }
 */
// ---------------------------------------------------------------------------
AnsiString __fastcall TImportaMedicao::FiltraCodigoRede(AnsiString cod_orig)
{
    // vari�veis locais
    int num_digito = 0;
    int num_letra = 0;
    char caractere;
    AnsiString cod_dest;
    int len = cod_orig.Length();

    // filtra c�digo, deixando apenas letras e d�gitos
    for (int ind = 1; ind <= len; ind++)
    {
        caractere = cod_orig[ind];
        if ((caractere >= 'A') && (caractere <= 'Z'))
        {
            cod_dest += caractere;
            num_letra++;
        }
        else if ((caractere >= 'a') && (caractere <= 'z'))
        {
            cod_dest += caractere;
            num_letra++;
        }
        else if ((caractere >= '0') && (caractere <= '9'))
        {
            cod_dest += caractere;
            num_digito++;
        }
    }
    // verifica se c�digo � formado por 3 letras e 4 d�gitos (padr�o Elpa)
    if ((num_letra == 3) && (num_digito == 4))
    { // verifica se o primeiro d�gito � zero
        if (cod_dest[4] == '0')
        { // elimina o primeiro d�gito do c�digo
            cod_dest.Delete(4, 1);
        }
    }
    return (cod_dest);
}

// ---------------------------------------------------------------------------
bool __fastcall TImportaMedicao::InsereImax(double valor[MAX_FASE], double valor_max,
    VTChave *chave)
{ // DVK 2016.05.25
    // variaveis locais
    VTCanal *canal;
    VTFases *fases = (VTFases*)apl_owner->GetObject(__classid(VTFases));
    VTPatamar *patamar, *patamar_max;
    VTPatamares *patamares = (VTPatamares*)apl_owner->GetObject(__classid(VTPatamares));
    int fase_tag = faseA;
    int nb = 0;
    double i_amp[MAX_FASE], i_med_amp[MAX_FASE];
    double fat_cor = 1.;
    double fat_cor_fase[MAX_FASE];

    // prote��o
    if (chave == NULL)
        return (false);
    if (!val_max)
        return (false);
    // verifica se o fluxo rodou para a rede
    if (!chave->rede->FluxoOK)
    {
        InsereMensagemLog("Fluxo n�o convergiu para a rede");
        return (false);
    }
    // determina patamar de demanda m�xima
    patamar_max = PatamarDemandaMax(chave);
    if (patamar_max == NULL)
        return (false);
    // verifica se tem valor por fase ou em 1 fase
    if (!IsDoubleZero(valor_max))
    { // verifica em qual fase a corrente � maior
        i_amp[indFASE_A] = Abs(chave->resflow->Ia_amp[nb][patamar_max->Index]);
        i_amp[indFASE_B] = Abs(chave->resflow->Ib_amp[nb][patamar_max->Index]);
        i_amp[indFASE_C] = Abs(chave->resflow->Ic_amp[nb][patamar_max->Index]);
        for (int ind_fase = indFASE_A; ind_fase <= indFASE_C; ind_fase++)
        {
            i_med_amp[ind_fase] = 0.;
            if (IsDoubleZero(chave->resflow->If_amp_max[nb][patamar_max->Index] - i_amp[ind_fase]))
            { // guarda fase e valor max medido
                fase_tag = fases->IndexToTag[ind_fase];
                i_med_amp[ind_fase] = valor_max;
                // determina fator de corre��o
                if (!IsDoubleZero(i_amp[ind_fase]))
                    fat_cor = Abs(valor_max / i_amp[ind_fase]);
                else
                    fat_cor = 1.;
            }
        }
        // determina a medi��o proporcional aos demais patamares e fases
        for (int ind_pat = 0; ind_pat < patamares->NumPatamar(); ind_pat++)
        {
            patamar = patamares->Patamar[ind_pat];
            i_med_amp[indFASE_A] = fat_cor * Abs(chave->resflow->Ia_amp[nb][patamar->Index]);
            i_med_amp[indFASE_B] = fat_cor * Abs(chave->resflow->Ib_amp[nb][patamar->Index]);
            i_med_amp[indFASE_C] = fat_cor * Abs(chave->resflow->Ic_amp[nb][patamar->Index]);
            InsereValorFase(i_med_amp, chave, patamar);
        }
    }
    else
    { // valida
        if (!ValidaValor(valor))
            return (false);
        // guarda valor do fluxo
        i_amp[indFASE_A] = Abs(chave->resflow->Ia_amp[nb][patamar_max->Index]);
        i_amp[indFASE_B] = Abs(chave->resflow->Ib_amp[nb][patamar_max->Index]);
        i_amp[indFASE_C] = Abs(chave->resflow->Ic_amp[nb][patamar_max->Index]);
        // determina fator de corre��o
        for (int ind_fase = indFASE_A; ind_fase <= indFASE_C; ind_fase++)
        {
            if (IsDoubleZero(i_amp[ind_fase]))
                fat_cor_fase[ind_fase] = 1.;
            else
                fat_cor_fase[ind_fase] = Abs(valor[ind_fase] / i_amp[ind_fase]);
        }
        // determina a medi��o proporcional aos demais patamares e fases
        for (int ind_pat = 0; ind_pat < patamares->NumPatamar(); ind_pat++)
        {
            patamar = patamares->Patamar[ind_pat];
            i_med_amp[indFASE_A] =
                fat_cor_fase[indFASE_A] * Abs(chave->resflow->Ia_amp[nb][patamar->Index]);
            i_med_amp[indFASE_B] =
                fat_cor_fase[indFASE_B] * Abs(chave->resflow->Ib_amp[nb][patamar->Index]);
            i_med_amp[indFASE_C] =
                fat_cor_fase[indFASE_C] * Abs(chave->resflow->Ic_amp[nb][patamar->Index]);
            InsereValorFase(i_med_amp, chave, patamar);
        }
    }
    return (true);
}

// ---------------------------------------------------------------------------
void __fastcall TImportaMedicao::InsereMensagemLog(AnsiString erro)
{
    // prote��o
    if (!log)
        return;
    // insere campos no log
    if (log)
        log->DefineValor("Tipo de Medi��o", TraduzTipoMedicao(tipo_canal));
    if (log)
        log->DefineValor("SED", campos->Strings[0]);
    if (log)
        log->DefineValor("SDMT", campos->Strings[1]);
    if (log)
        log->DefineValor("Chave", campos->Strings[2]);
    // verifica se h� patamar
	if (val_max)
	{
        if (log)
			log->DefineValor("Patamar", "-");
	}
	else
	{
		if (log)
			log->DefineValor("Patamar", campos->Strings[3]);
    }

    // if (log) log->DefineValor("Descri��o",       erro);
    if (log)
        log->Add(erro);
    // atualiza contador de linhas do log
    linhas_log++;
}

// ---------------------------------------------------------------------------
bool __fastcall TImportaMedicao::InsereValorFase(double valor[MAX_FASE], VTChave *chave,
    VTPatamar *patamar)
{
    // variaveis locais
    VTCanal *canal;
    VTFases *fases = (VTFases*)apl_owner->GetObject(__classid(VTFases));
    VTMedidor *medidor;
    VTPatamares *patamares = (VTPatamares*)apl_owner->GetObject(__classid(VTPatamares));

    // prote��o
    if (!chave)
        return (false);
    if (!patamar)
        return (false);
    // verifica se existe Medidor p/ chave
    if ((medidor = medicoes->ExisteMedidor(chave)) == NULL)
    { // cria um novo objeto VTMedidor
        medidor = medicoes->InsereMedidor(chave);
        if (medidor == NULL)
            return (false);
        medicoes->LisMedidor()->Add(medidor);
    }
    // converte unidade
    ConverteUnidade(valor);
    // valida valores
    if (!ValidaValor(valor))
        return (false);
    for (int ind_fase = indFASE_A; ind_fase <= indFASE_C; ind_fase++)
    { // verifica se existe canal da grandeza com cada uma das fases
        canal = medidor->ExisteCanal(chave, tipo_canal, fases->IndexToTag[ind_fase]);
        if (!canal)
        { // cria canal
            canal = medidor->InsereCanal(chave, tipo_canal, fases->IndexToTag[ind_fase]);
            CompletaDadosCanal(canal);
        }
        if (!canal->Enabled)
            canal->Enabled = true;
        // verifica se a escala da curva � a mesma
        if (patamares->Escala != canal->Curva->Escala)
            canal->Curva->Reconfigura(patamares->Escala);
        canal->ValorMedido[patamar] = valor[ind_fase];
    }
    return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TImportaMedicao::InsereValorMaxFase(double valor_max, VTChave *chave,
    VTPatamar *patamar)
{ // DVK 2016.03.25
    // variaveis locais
    VTCanal *canal;
    VTFases *fases = (VTFases*)apl_owner->GetObject(__classid(VTFases));
    VTPatamares *patamares = (VTPatamares*)apl_owner->GetObject(__classid(VTPatamares));
    int fase_tag = faseA;
    int nb = 0;
    double i_amp[MAX_FASE], i_med_amp[MAX_FASE];
    double fat_cor = 1.;

    // prote��o
    if (!chave)
        return (false);
    if (!patamar)
        return (false);
    if (IsDoubleZero(valor_max))
        return (false);
    // verifica em qual fase a corrente � maior
    i_amp[indFASE_A] = Abs(chave->resflow->Ia_amp[nb][patamar->Index]);
    i_amp[indFASE_B] = Abs(chave->resflow->Ib_amp[nb][patamar->Index]);
    i_amp[indFASE_C] = Abs(chave->resflow->Ic_amp[nb][patamar->Index]);
    for (int ind_fase = indFASE_A; ind_fase <= indFASE_C; ind_fase++)
    {
        i_med_amp[ind_fase] = 0.;
        if (IsDoubleZero(i_amp[ind_fase]))
        {
            fase_tag = fases->IndexToTag[ind_fase];
            InsereMensagemLog("Valor nulo do fluxo na fase " + fases->AsStrABCN[fase_tag]);
            continue;
        }
        if (IsDoubleZero(chave->resflow->If_amp_max[nb][patamar->Index] - i_amp[ind_fase]))
        { // guarda fase e valor max medido
            fase_tag = fases->IndexToTag[ind_fase];
            i_med_amp[ind_fase] = valor_max;
            // determina fator de corre��o
            fat_cor = Abs(valor_max / i_amp[ind_fase]);
        }
    }
    // determina a medi��o proporcional �s demais fases
    for (int ind_fase = indFASE_A; ind_fase <= indFASE_C; ind_fase++)
    {
        if (!IsDoubleZero(i_med_amp[ind_fase]))
            continue;
        i_med_amp[ind_fase] = fat_cor * i_amp[ind_fase];
    }
    return (InsereValorFase(i_med_amp, chave, patamar));
}

// ---------------------------------------------------------------------------
bool __fastcall TImportaMedicao::InsereValorTri(double valor_tri, VTChave *chave,
    VTPatamar *patamar)
{
    // variaveis locais
    VTCanal *canal;
    VTMedidor *medidor;
    VTPatamares *patamares = (VTPatamares*)apl_owner->GetObject(__classid(VTPatamares));

    // prote��o
    if (!chave)
        return (false);
    if (!patamar)
        return (false);
    // verifica se existe Medidor p/ chave
    if ((medidor = medicoes->ExisteMedidor(chave)) == NULL)
    { // cria um novo objeto VTMedidor
        medidor = medicoes->InsereMedidor(chave);
        if (medidor == NULL)
            return (false);
        medicoes->LisMedidor()->Add(medidor);
    }
    // valida valor
    if (IsDoubleZero(valor_tri))
    {
        InsereMensagemLog("Valor nulo no patamar");
        return (false);
    }
    // verifica o tipo do canal para converter unidade
    if ((tipo_canal == canalP) || (tipo_canal == canalQ))
    {
        valor_tri /= 1000.; // kW => MW ou kvar => Mvar
    }
    // verifica se existe canal da grandeza trif�sica
    if ((canal = medidor->ExisteCanal(chave, tipo_canal, faseABC)) == NULL)
    { // cria canal abc
        canal = medidor->InsereCanal(chave, tipo_canal, faseABC);
        CompletaDadosCanal(canal);
    }
    if (!canal->Enabled)
        canal->Enabled = true;
    // verifica se a escala da curva � a mesma
    if (patamares->Escala != canal->Curva->Escala)
        canal->Curva->Reconfigura(patamares->Escala);
    canal->ValorMedido[patamar] = valor_tri;

    return (true);
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/18381986
// https://sinapsisenergia.teamwork.com/#/tasks/18630318
bool __fastcall TImportaMedicao::LeArquivoCurvaPadrao(TStrings *files)
{
    // DVK 2015.11.12 leitura de arquivo padronizado
    // vari�veis locais
    AnsiString  pathname, horamin;
    AnsiString  linha, cod_etd, cod_pri, cod_chave;
    double      valor[MAX_FASE];
    double      valor_tri;
    strHM       hm_ini;
    strHM       hm_ini_tratada;
    VTChave     *chave;
    VTMedidor   *medidor;
    VTPatamar   *patamar;
    VTPatamares *patamares = (VTPatamares*)apl_owner->GetObject(__classid(VTPatamares));

    // arquivo com campos esperados: importante, o c�digo da chave pode n�o existir
    // separador de tipos de arquivo:
    // "Tipo da Medi��o"; <tipo>
    // <tipo>   = {I, Imax, VF, VL, Pabc, P3f, Qabc, Q3f,  S}
    // unidades = {A, A,    kV, kV, kW,   kW,  kvar, kvar, kVA}
    // formato1, sem c�digo da Chave: NomeDaETD;NomeDoCircuito;            HH:MM;FaseA;FaseB;FaseC;FaseABC
    // formato2, com c�digo da Chave: NomeDaETD;NomeDoCircuito;NomeDaChave;HH:MM;FaseA;FaseB;FaseC;FaseABC

    // DVK 2016.03.25 valor de corrente em 1 fase indefinida: coluna FaseABC
    // ser� o valor da maior corrente em uma das fases e os demais ser�o
    // proporcionais ao valor do fluxo

    // DVK 2016.05.25 se for Imax, o campo com hora/minuto dever� estar vazio
    // poder� ser 1 valor (na coluna FaseABC) ou 3 valores (em cada fase)
    // se for 1 valor, ser� o valor da maior corrente em uma das fases e os demais
    // ser�o proporcionais ao valor do fluxo

    try
    {
        // verifica se o Log est� habilitado
        if (LogEnabled)
        {
            // cria e inicia objeto Log
            log = DLL_NewObjLog(apl_owner);
            LogInicia();
        }
        // obt�m objetos
        medicoes = (VTMedicoes*)apl_owner->GetObject(__classid(VTMedicoes));
        radial   = (VTRadial*)apl_owner->GetObject(__classid(VTRadial));
        //preenche lista de redes validas
        lisREDE_VAL->Clear();
        medicoes->PreencheLisRedeVal(lisREDE_VAL);
        // inicia log
        LogReinicia();
        linhas_log = 0;
        // loop p/ todos arquivos selecionados
        for (int nf = 0; nf < files->Count; nf++)
        {
            pathname = files->Strings[nf];
            // prote��o: verifica se o arquivo existe
            if (!FileExists(pathname))
            {
                LogFecha();
                return (false);
            }
            // carrega o arquivo em  lines
            lines->LoadFromFile(pathname);
            // extrai informa��es das linhas
            for (int n = 0; n < lines->Count; n++)
            {
                linha = lines->Strings[n];
                // Verifica coment�rios e outros dados n�o v�lidos
                if (VerificaComentario(linha))
                    continue;
                // verifica se � v�lido o tipo de canal
                if (tipo_canal == canalINDEF)
                    continue;
                // extrai campos da linha
                if (ExtraiCampos(linha, ";") < 8)
                    continue;
                // se houver 8 campos, insere o c�digo da Chave como string nulo
                if (campos->Count == 7)
                    campos->Insert(2, "");
                // determina c�digo da etd, prim�rio e chave
                cod_etd = campos->Strings[0].Trim();
                cod_pri = campos->Strings[1].Trim();
                cod_chave = campos->Strings[2].Trim();
//
// ESSE BLOCO SER� ADICIONADO AP�S MODIFICAR O FORMATO DO ARQUIVO DE MEDI��ES
// D.V.C 07/02/2019
//                // verifica se a subestacao est� null
//                if(cod_etd.IsEmpty())
//                    continue;
//                // Verifica se essa medi��o � da subesta��o carregada
//                if(ExisteSubestacao(cod_etd) == NULL)
//                    continue;
//
                // verifica se tanto cod_pri como cod_chave est�o nulos
                if (cod_pri.IsEmpty() && cod_chave.IsEmpty())
                    continue;
                // verifica se somente cod_pri est� nulo
                if (cod_pri.IsEmpty() && (!cod_chave.IsEmpty()))
                {
                    chave = ExisteChave(cod_pri, cod_chave);
                }
                else
                {
                    // primeiro verifica se a medi��o se refere a algum prim�rio da rede
                    // se n�o houver, simplesmente ignora pois a rede pode n�o ter sido lida do arquivo MDB
                    if (ExistePrimarioCodigoRede(cod_pri) == NULL)
                        continue;
                    // se a medi��o se refere a algum prim�rio da rede verifica se � v�lido
                    // ou seja, se n�o foi equivalentado, se tem fluxo e este n�o � 0
                    // se for inv�lido emite mensagem ao usu�rio antes de passar ao pr�ximo
                    else if (ExistePrimario(cod_pri) == NULL)
                    {
                        InsereMensagemLog("Prim�rio inv�lido");
                        continue;
                    }
                    // finalmente, se a medi��o se refere a algum prim�rio da rede
                    // e esse prim�rio � v�lido (n�o � equivalente e tem fluxo <> 0)
                    // recupera a qual chave se refere
                    chave = ExisteChave(cod_pri, cod_chave);
                }
                // prote��o: verifica se existe a chave
                if (chave == NULL)
                {
                    InsereMensagemLog("Chave n�o encontrada");
                    continue;
                }
                // l� hora/minuto e define patamar
                horamin = campos->Strings[3].Trim();
                if (!horamin.IsEmpty())
                {
                    // l� hora e minuto
                    hm->Clear();
                    if (ExtraiStrings(horamin, ":", hm) != 2)
                        continue;
                    hm_ini.hora   = StrToInteger(hm->Strings[0].Trim());
                    hm_ini.minuto = StrToInteger(hm->Strings[1].Trim());
                    //solu��o simplista errada... =/
                    // verifica se a hora � 24 (muda para 0)
//                    if (hm_ini.hora == 24)
//                        hm_ini.hora = 0;
//                    patamar = patamares->ExistePatamar(hm_ini.hora, hm_ini.minuto);
                    //solu��o do Kenndy
                    hm_ini_tratada = TrataHoraInteira(hm_ini); //FKM 2017.04.11
                    patamar        = patamares->ExistePatamar(hm_ini_tratada.hora, hm_ini_tratada.minuto);
                }
                else
                    patamar = NULL;
                // verifica valores por fase
                valor[indFASE_A] = StrToDouble(campos->Strings[4].Trim());
                valor[indFASE_B] = StrToDouble(campos->Strings[5].Trim());
                valor[indFASE_C] = StrToDouble(campos->Strings[6].Trim());
                valor_tri        = StrToDouble(campos->Strings[7].Trim());
                // caso especial: patamar NULL para corrente m�xima
                if (patamar == NULL)
                    InsereImax(valor, valor_tri, chave);
                // insere valor no canal do medidor, para cada fase ou trif�sico no patamar
                else
                    switch (tipo_canal)
                    {
                    case canalS:
                    case canalV:
                    case canalI:
                        // valor por patamar
                        if (IsDoubleZero(valor_tri))
                            InsereValorFase(valor, chave, patamar);
                        else
                            InsereValorMaxFase(valor_tri, chave, patamar);
                        break;
                    case canalP:
                    case canalQ:
                        if (por_fase)
                            InsereValorFase(valor, chave, patamar);
                        else
                            InsereValorTri(valor_tri, chave, patamar);
                        break;
                    default:
                        break;
                    }
            }
            // corrige a barra dos medidores criados, considerando arvore de blocos
            medicoes->AcertaBarraMedidores();
            // mostra log
            LogFecha();
            if ((log != NULL) && (linhas_log > 0))
                log->ShowModal("");
        }
    }
    catch (Exception &e)
    {
        LogFecha();
        return (false);
    }
    return (true);
}

// ---------------------------------------------------------------------------
void __fastcall TImportaMedicao::LogFecha(void)
{
    // fecha Log
    if (log)
        log->Close();
}

// ---------------------------------------------------------------------------
void __fastcall TImportaMedicao::LogInicia(void)
{
    // variaveis locais
    VTPath *path = (VTPath*)apl_owner->GetObject(__classid(VTPath));

    if (!log)
        return;
    // inicia Log
    if (log)
        log->Open(path->DirTmp() + "\\ImportaMedicao.log");
    if (log)
        log->InsereCampo("Tipo de Medi��o");
    if (log)
        log->InsereCampo("SED");
    if (log)
        log->InsereCampo("SDMT");
    if (log)
        log->InsereCampo("Chave");
    if (log)
        log->InsereCampo("Patamar");
    if (log)
        log->InsereCampo("Descri��o");
}

// ---------------------------------------------------------------------------
void __fastcall TImportaMedicao::LogReinicia(void)
{
    // variaveis locais
    VTPath *path = (VTPath*)apl_owner->GetObject(__classid(VTPath));

    if (!log)
        return;
    // inicia Log
    if (log)
        log->Open(path->DirTmp() + "\\ImportaMedicao.log");
    if (log)
        log->DefineValor("Tipo de Medi��o", "");
    if (log)
        log->DefineValor("SED", "");
    if (log)
        log->DefineValor("SDMT", "");
    if (log)
        log->DefineValor("Chave", "");
    if (log)
        log->DefineValor("Patamar", "");
    if (log)
        log->DefineValor("Descri��o", "");
}

// ---------------------------------------------------------------------------
VTPatamar* __fastcall TImportaMedicao::PatamarDemandaMax(VTChave *chave)
{
    // vari�veis locais
    int nb = 0;
    complex<double>pq_mva, s_mva;
    VTPatamar *patamar_max = NULL;
    VTPatamares *patamares = (VTPatamares*)apl_owner->GetObject(__classid(VTPatamares));

    // zera demanda m�xima
    s_mva = CZero();
    // determina indice da barra onde o fluxo � positivo
    if (chave->resflow->SentidoP[nb][0] == fluxoNEG)
        nb = 1;
    // loop para todos patamares
    for (int ind_pat = 0; ind_pat < patamares->NumPatamar(); ind_pat++)
    {
        // totaliza demanda de todas Cargas no patamar
        pq_mva = CZero();
        pq_mva += chave->resflow->S_mva[nb][ind_pat];
        // verifica valor m�ximo
        if ((ind_pat == 0) || (Abs(pq_mva) > Abs(s_mva)))
        {
            s_mva = pq_mva;
            patamar_max = patamares->Patamar[ind_pat];
        }
    }
    return (patamar_max);
}

// ---------------------------------------------------------------------------
int __fastcall TImportaMedicao::TraduzTipoMedicao(AnsiString tipo_med)
{
    // verifica o tipo de medi��o
    if (tipo_med.IsEmpty())
        return (canalINDEF);
    if (tipo_med.AnsiCompareIC("S") == 0)
        return (canalS);
    if (tipo_med.AnsiCompareIC("I") == 0)
    {
        val_max = false;
        return (canalI);
    }
    if (tipo_med.AnsiCompareIC("Imax") == 0)
    {
        val_max = true;
        return (canalI);
    }
    if (tipo_med.AnsiCompareIC("P3f") == 0)
    {
        por_fase = false;
        return (canalP);
    }
    if (tipo_med.AnsiCompareIC("Pabc") == 0)
    {
        por_fase = true;
        return (canalP);
    }
    if (tipo_med.AnsiCompareIC("Q3f") == 0)
    {
        por_fase = false;
        return (canalQ);
    }
    if (tipo_med.AnsiCompareIC("Qabc") == 0)
    {
        por_fase = true;
        return (canalQ);
    }
    if (tipo_med.AnsiCompareIC("VF") == 0)
    {
        por_fase = true;
        return (canalV);
    }
    if (tipo_med.AnsiCompareIC("VL") == 0)
    {
        por_fase = false;
        return (canalV);
    }
    return (canalINDEF);
}

// ---------------------------------------------------------------------------
AnsiString __fastcall TImportaMedicao::TraduzTipoMedicao(int tipo_med)
{
    // verifica o tipo de medi��o
    switch (tipo_med)
    {
    case canalI:
        if (val_max)
            return ("I m�ximo");
        else
            return ("I por patamar");
    case canalS:
        return ("S");
    case canalP:
        if (por_fase)
            return ("P por fase");
        else
            return ("P trif�sico");
    case canalQ:
        if (por_fase)
            return ("Q por fase");
        else
            return ("Q trif�sico");
    case canalV:
        if (por_fase)
            return ("V de linha");
        else
            return ("V de fase");
    default:
        return ("");
    }
}

//---------------------------------------------------------------------------
//FKM 2017.04.11 - O Sinap trata os intervalos dos patamares como
// Hora_inicio e Hora_fim , sendo o intervalo fechado no inicio e aberto no final
// mas os medidores e a l�gica do usu�rio consideram aberto no inicio e fechado no final,
// isto �, o valor gravado pelo medidor na hora 01:00 � o valor que ele observou entre
// 00:01 e 01:00, portanto sendo referente ao patamar [00:00;01:00[ , por isso �
// necess�rio "burlar" isso
//---------------------------------------------------------------------------
strHM __fastcall TImportaMedicao::TrataHoraInteira(strHM hora_minuto)
   {//variaveis locais
   strHM nova_hora_minuto;

   //verifica se � uma hora inteira
   if(hora_minuto.minuto == 0)
      {//caso seja 00:00
      if(hora_minuto.hora == 0)
         {
         nova_hora_minuto.hora = 23;
         }
      else
         {
         nova_hora_minuto.hora = hora_minuto.hora - 1;
         }
      nova_hora_minuto.minuto = 59;
      }
   else
      {
      nova_hora_minuto = hora_minuto;
      }

   return nova_hora_minuto;
   }

// ---------------------------------------------------------------------------
bool __fastcall TImportaMedicao::ValidaValor(double valor[MAX_FASE])
{
    // variaveis locais
    int nulo = 0;

    // verifica se todos os valores sao nulos
    for (int nv = 0; nv < MAX_FASE; nv++)
    {
        if (IsDoubleZero(valor[nv]))
            nulo++;
    }
    if (nulo >= (MAX_FASE - 1))
    {
        InsereMensagemLog("Valor nulo no patamar");
        return (false);
    }
    return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TImportaMedicao::VerificaComentario(AnsiString linha)
{
    if (linha.IsEmpty())
        return (true);
    if (linha.Length() < 2)
        return (false);
    if ((linha[1] == '/') && (linha[2] == '/'))
    { // � linha de coment�rio e pode conter o tipo da medi��o
        if (ExtraiCampos(linha, ":;") < 2)
            return (true);
        // guarda tipo de medi��o
        tipo_canal = TraduzTipoMedicao(campos->Strings[1].Trim());
        return (true);
    }
    return (false);
}

// -----------------------------------------------------------------------------
// eof
