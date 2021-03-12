#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <DateUtils.hpp>
#include <Math.hpp>
#include <Fontes\Rede\VTEqpto.h>
#include <Fontes\Rede\VTRede.h>
#include <Fontes\Funcao\TFormMsgBox.h>
#include <DLL_Inc\Funcao.h>
#pragma argsused

// -----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
{
	return 1;
}

// -----------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/14569628
EXPORT void __fastcall Alerta(AnsiString txt)
{
	// vari�veis locais
	TAction *ActionAlerta = (TAction*)(Application->Tag);

	if (ActionAlerta)
	{
		ActionAlerta->Caption = txt;
//		ActionAlerta->Tag = (int)NULL;
		ActionAlerta->Hint = "";
		ActionAlerta->Execute();
	}
}
// -----------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/14569628
//https://sinapsisenergia.teamwork.com/#tasks/16258668
EXPORT void __fastcall AlertaMalha(VTRede *rede)
{
	// vari�veis locais
	TAction *ActionAlerta = (TAction*)(Application->Tag);
	AnsiString txt;

	if (rede != NULL)
	{
		txt = rede->Codigo + ": exist�ncia de malha em rede definida como radial";
		if (ActionAlerta)
		{
			ActionAlerta->Caption = txt;
//			ActionAlerta->Tag = (int)rede;
			try
			{//IMPORTANTE: o objeto foi guardado no Hint como string, pois em 64 bits,
			//o endere�o do ponteiro n�o cabe num inteiro (ActionAlerta->Tag)
				#ifdef _WIN64
				__int64 obj = __int64(rede);
				ActionAlerta->Hint = IntToStr(obj);
				#endif

				#ifdef __WIN32__
				int obj = int(rede);
				ActionAlerta->Hint = IntToStr(obj);
				#endif
			}
			catch(...)
			{
				ActionAlerta->Hint = "";
            }
			ActionAlerta->Execute();
		}
	}
}
// ---------------------------------------------------------------------------
EXPORT AnsiString __fastcall ASCIIFilter(AnsiString txt)
{
	// vari�veis locais
	WideString wstr = txt;
	int wstr_len = wstr.Length();
	unsigned char car_orig, car_dest;
	unsigned char car_stuf = '_';

	struct strASCII
	{
		unsigned char orig, dest;
	} tab_ascii[] =
	{ {'�', 'a'}, {'�', 'a'}, {'�', 'a'}, {'�', 'a'}, {'�', 'a'}, {'�', 'A'}, {'�', 'A'}, {'�', 'A'
		}, {'�', 'A'}, {'�', 'A'}, {'�', 'e'}, {'�', 'e'}, {'�', 'e'}, {'�', 'e'}, {'�', 'E'},
		{'�', 'E'}, {'�', 'E'}, {'�', 'E'}, {'�', 'i'}, {'�', 'i'}, {'�', 'i'}, {'�', 'I'},
		{'�', 'I'}, {'�', 'I'}, {'�', 'o'}, {'�', 'o'}, {'�', 'o'}, {'�', 'o'}, {'�', 'o'},
		{'�', 'O'}, {'�', 'O'}, {'�', 'O'}, {'�', 'O'}, {'�', 'O'}, {'�', 'u'}, {'�', 'u'},
		{'�', 'u'}, {'�', 'u'}, {'�', 'U'}, {'�', 'U'}, {'�', 'U'}, {'�', 'U'}, {'�', 'c'},
		{'�', 'C'}, {'�', 'n'}, {'�', 'N'}, {'�', '2'}};
	// filtra caracteres inadequados da tabela ASCII
	for (int i = 1; i <= wstr_len; i++)
	{
		car_orig = (unsigned char)wstr[i];
		if (car_orig < 32)
		{
			wstr[i] = car_stuf;
		}
		else if (car_orig <= 126)
		{
			continue;
		}
		else
		{ // tenta substitui caracteres
			car_dest = car_stuf;
			for (int j = 0; j < sizeof(tab_ascii) / sizeof(strASCII); j++)
			{
				if (car_orig == tab_ascii[j].orig)
				{
					car_dest = tab_ascii[j].dest;
					break;
				}
			}
			wstr[i] = car_dest;
		}
	}
	txt = wstr;
	return (txt);
}
// ---------------------------------------------------------------------------
EXPORT AnsiString __fastcall ASCIIFilterWithAccents(AnsiString text)
{
    // vari�veis locais
	WideString wstr = text;
	int wstr_len = wstr.Length();
	unsigned char car_orig;
	unsigned char car_stuf = '_';


	// filtra caracteres inadequados da tabela ASCII
	for (int i = 1; i <= wstr_len; i++)
	{
		car_orig = (unsigned char)wstr[i];
		if ((car_orig >= 32) && (car_orig <= 377))
		{
			continue;
		}
		else
		{
			wstr[i] = car_stuf;
		}
	}
	//Aviso(texto);
	text = wstr;
	return (text);
}
/*
 //---------------------------------------------------------------------------
 EXPORT void __fastcall Aviso(AnsiString txt)
 {
 //vari�veis locais
 WideString aviso   = txt;
 WideString caption = "AVISO!";

 MessageBox(NULL, aviso.c_bstr(), caption.c_bstr(), MB_TOPMOST+MB_OK+MB_ICONWARNING);
 }
 */
// ---------------------------------------------------------------------------
EXPORT void __fastcall Aviso(AnsiString txt)
{
	// vari�veis locais
	TFormMsgBox *MsgBox;

	MsgBox = new TFormMsgBox(NULL);
	MsgBox->Aviso(txt);
	MsgBox->ShowModal();
	delete MsgBox;
}

// ---------------------------------------------------------------------------
EXPORT AnsiString __fastcall ComplexToStr(AnsiString fmt, double real, double imag, byte separador)
{
	// vari�veis locais
	AnsiString txt;

	txt = DoubleToStr(fmt, real, separador);
	txt += (imag >= 0) ? "+j" : "-j";
	txt += DoubleToStr(fmt, fabs(imag), separador);
	return (txt);
}

// -----------------------------------------------------------------------------
EXPORT AnsiString __fastcall ComplexPolarToStr(AnsiString fmt, double modulo, double fase,
	byte separador)
{
	// vari�veis locais
	AnsiString txt;

	txt = DoubleToStr(fmt, modulo, separador);
	txt += DoubleToStr("[%2.1f]", RadToDeg(fase), separador);
	return (txt);
}

/*
 //---------------------------------------------------------------------------
 EXPORT int __fastcall Confirma(AnsiString txt1, AnsiString txt2, int buttons)
 {
 //vari�veis locais
 int resp;
 WideString msg;
 WideString caption = "Aten��o!";

 //valor default de buttons = -1 -> exibe bot�es Sim e N�o
 //se diferente de -1, exibe os bot�es passados para a fun��o
 //concatena os dois strings
 msg = txt1 + "\n\n" + txt2;
 //resp = Application->MessageBox(txt1.c_str(), "Aten��o!",
 //                               MB_TOPMOST+MB_YESNOCANCEL+MB_ICONWARNING);

 if (buttons == -1)
 {//mensagem default com bot�es Sim e N�o
 resp = MessageBox(NULL, msg.c_bstr(), caption.c_bstr(), MB_TOPMOST+MB_YESNO+MB_ICONWARNING);
 }
 else
 {//mensagem personalizada com os bot�es passados para a fun��o
 resp = MessageBox(NULL, msg.c_bstr(), caption.c_bstr(), MB_TOPMOST+buttons+MB_ICONWARNING);
 }
 return(resp);
 }
 */
// ---------------------------------------------------------------------------
EXPORT int __fastcall Confirma(AnsiString txt1, AnsiString txt2, int buttons)
{
	// vari�veis locais
	int resp;
	TFormMsgBox *MsgBox;

	MsgBox = new TFormMsgBox(NULL);
	MsgBox->Confirma(txt1, txt2, buttons);
	switch (MsgBox->ShowModal())
	{
	case mrNo:
		resp = IDNO;
		break;
	case mrYes:
		resp = IDYES;
		break;
	case mrCancel:
	default:
		resp = IDCANCEL;
		break;
	}
	delete MsgBox;
	return (resp);
}

// -----------------------------------------------------------------------------
EXPORT void __fastcall CopiaTList(TList *lisORI, TList *lisDES)
{
	// vari�veis locais
	void *object;

	// prote��o
	if ((lisORI == NULL) || (lisDES == NULL))
		return;
	//verifica se a lista destino est� vazia: copia direto
	if(lisDES->Count == 0)
	{
		lisDES->Assign(lisORI, laCopy);
		return;
    }
	// copia objetos de lisORI p/ lisDES
	for (int n = 0; n < lisORI->Count; n++)
	{
		object = lisORI->Items[n];
		if (lisDES->IndexOf(object) < 0)
			lisDES->Add(object);
	}
}

// -----------------------------------------------------------------------------
EXPORT AnsiString __fastcall DataToDoubleToStr(TDate date)
{
	return (DoubleToStr("%10.5f", double(date)));
}

// ---------------------------------------------------------------------------
EXPORT void __fastcall DestroiTObject(TObject *object)
{
	if (object != NULL)
	{
		delete object;
		object = NULL;
	}
}

// ---------------------------------------------------------------------------
EXPORT int __fastcall Distancia_cm(int x1, int y1, int x2, int y2)
{
	// vari�veis locais
	double dx, dy, dist;

	try
	{
		dx = (x2 - x1) / 100.;
		dy = (y2 - y1) / 100.;
		dist = sqrt((dx * dx) + (dy * dy));
	}
	catch (Exception &e)
	{
		return (0);
	}
	return ((int)(dist * 100.));
}

// ---------------------------------------------------------------------------
EXPORT int __fastcall Distancia_m(int x1, int y1, int x2, int y2)
{
	return (int(Distancia_cm(x1, y1, x2, y2) / 100.));
}
/*
// ---------------------------------------------------------------------------
EXPORT AnsiString __fastcall DoubleToStr(AnsiString fmt, double dval, byte separador)
{
	// vari�veis locais
	AnsiString sval;
	byte orig_separador;

	// verifica se foi definido um separador espec�fico
	if ((separador == '.') || (separador == ','))
	{ // salva separador inicial
		orig_separador = FormatSettings.DecimalSeparator;
		// redefine separador decimal
		FormatSettings.DecimalSeparator = separador;
		// converte double to string
		sval = Format(fmt, ARRAYOFCONST((dval)));
		// restaura separador original
		FormatSettings.DecimalSeparator = orig_separador;
	}
	else
	{ // usa o separador configurado no sistema
		sval = Format(fmt, ARRAYOFCONST((dval)));
	}

	return (sval);
}
*/
//---------------------------------------------------------------------------
EXPORT AnsiString __fastcall DoubleToStr(AnsiString fmt, double dval, byte separador)
{
	// vari�veis locais
	byte orig_separador;
	int length;
	AnsiString sval;

	// determina separador configurado no sistam
	orig_separador = FormatSettings.DecimalSeparator;
	// usa o separador configurado no sistema
	sval = Format(fmt, ARRAYOFCONST((dval)));
	// verifica se foi definido um separador diferente do configurado no sistema
	if (separador != orig_separador)
	{ // subsitui separador original no string
		length = sval.Length();
		for (int n = 1; n <= length; n++)
		{
			if (sval[n] == orig_separador)
			{
				sval[n] = separador;
				break;
			}
		}
	}
	return (sval);
}

// ---------------------------------------------------------------------------
EXPORT void __fastcall Erro(AnsiString txt)
{
	// vari�veis locais
	WideString erro = txt;
	WideString caption = "ERRO!";

	MessageBox(NULL, erro.c_bstr(), caption.c_bstr(), MB_TOPMOST + MB_OK + MB_ICONERROR);
}

/*
 //---------------------------------------------------------------------------
 EXPORT void __fastcall Erro(AnsiString txt)
 {
 //vari�veis locais
 TFormMsgBox *MsgBox;

 MsgBox = new TFormMsgBox(NULL);
 MsgBox->Erro(txt);
 MsgBox->ShowModal();
 delete MsgBox;
 }
 */
// ---------------------------------------------------------------------------
EXPORT TForm* __fastcall ExisteForm(AnsiString class_name, TComponent *Owner)
{
	// vari�veis locais
	AnsiString form_name;
	TForm *form;

	// verifica se existe um Form com o nome indicado
	for (int n = 0; n < Screen->FormCount; n++)
	{
		form_name = Screen->Forms[n]->ClassName();
		if (class_name.AnsiCompareIC(form_name) == 0)
		{
			if ((Owner == NULL) || (Screen->Forms[n]->Owner == Owner))
				return (Screen->Forms[n]);
		}
	}
	return (NULL);
}

// ---------------------------------------------------------------------------
EXPORT int __fastcall ExtraiStrings(AnsiString txt, AnsiString separadores, TStrings *campos)
{
	// vari�veis locais
	int ind_ini, ind_fim;
	int len = txt.Length();

	// reinicia campos
	campos->Clear();
	// loop de identifica��o dos campos
	ind_ini = 1;
	while (ind_ini <= len)
	{ // determina fim de um campo
		ind_fim = ind_ini;
		while (ind_fim <= len)
		{ // procura por delimitador
			if (separadores.AnsiPos(txt[ind_fim]) > 0)
				break;
			// incrementa ind_fim
			ind_fim++;
		}
		// encontrou delimitador ou fim do string
		if (ind_fim == ind_ini)
		{ // campo vazio
			campos->Add("");
		}
		else if (ind_fim > ind_ini)
		{ // extrai substring
			campos->Add(txt.SubString(ind_ini, ind_fim - ind_ini));
		}
		// atualiza ind_ini
		ind_ini = ind_fim + 1;
	}
	return (campos->Count);
}

// ---------------------------------------------------------------------------
EXPORT int __fastcall FalsoId(void)
{
	// vari�veis locais
	static int id = 0;
	// atualiza id
	id++;
	return (-id);
}

// -----------------------------------------------------------------------------
EXPORT AnsiString __fastcall FormataData(TDateTime dtt)
{
	// vari�veis locais
	unsigned short year, month, day;
	AnsiString strData;

	dtt.DecodeDate(&year, &month, &day);
	strData.sprintf("%04d.%02d.%02d", year, month, day);

	return (strData);
}

// -----------------------------------------------------------------------------
EXPORT AnsiString __fastcall FormataDataHora(TDateTime dtt)
{
	return (FormataData(dtt) + " " + FormataHora(dtt));
}

// -----------------------------------------------------------------------------
EXPORT AnsiString __fastcall FormataHora(TDateTime dtt)
{
	// vari�veis locais
	unsigned short hour, min, sec, msec;
	AnsiString strHora;

	dtt.DecodeTime(&hour, &min, &sec, &msec);
	strHora.sprintf("%02d:%02d:%02d:%03d", hour, min, sec, msec);

	return (strHora);
}

// -----------------------------------------------------------------------------
EXPORT TObject* __fastcall GetEqptoObj(VTEqpto *eqpto, AnsiString obj_class_name)
{
	TMetaClass *mObjClass;
	AnsiString mObjClassName;
	TObject *mObj = NULL;

	if (eqpto == NULL)
	{
		return mObj;
	}
	if (eqpto->Obj != NULL)
	{
		mObjClass = eqpto->Obj->ClassType();
		mObjClassName = mObjClass->ClassName();
		if (mObjClassName.AnsiCompareIC(obj_class_name) == 0)
		{
			mObj = eqpto->Obj;
		}
	}

	return mObj;
}

// -----------------------------------------------------------------------------
EXPORT bool __fastcall IsComplexZero(double real, double imag, double precisao)
{
	return (IsDoubleZero(real, precisao) && IsDoubleZero(imag, precisao));
}

// -----------------------------------------------------------------------------
EXPORT bool __fastcall IsDoubleZero(double valor, double precisao)
{
	// verifica se o valor � menor que a precis�o
	return (fabs(valor) <= precisao);
}

// -----------------------------------------------------------------------------
EXPORT void __fastcall LimpaTList(TList *lisOBJ)
{
	// vari�veis locais
	TObject *obj;

	try
	{ // elimina todos os objetos de TList
		for (int n = 0; n < lisOBJ->Count; n++)
		{
			try
			{
				if ((obj = (TObject*)lisOBJ->Items[n]) != NULL)
				{
					delete obj;
				}
			}
			catch (Exception &)
			{ // nada a fazer
			}
		}
		// limpa o conte�do de TList
		lisOBJ->Clear();
	}
	catch (Exception &e)
	{ // nada a fazer
	}
}

// -----------------------------------------------------------------------------
EXPORT bool __fastcall Lista1ContemLista2(TList *lisOBJ1, TList *lisOBJ2)
{
	// vari�veis locais
	TObject *obj;

	// prote��o
	if ((lisOBJ1 == NULL) || (lisOBJ2 == NULL))
		return (false);
	// prote��o: lista vazia � tratada como N�O contida
	if (lisOBJ2->Count == 0)
		return (false);
	// verifica lisOBJ1 cont�m todos objetos de lisOBJ2
	for (int n = 0; n < lisOBJ2->Count; n++)
	{
		obj = (TObject*)lisOBJ2->Items[n];
		if (lisOBJ1->IndexOf(obj) < 0)
			return (false);
	}
	return (true);
}

// -----------------------------------------------------------------------------
EXPORT bool __fastcall Lista1IgualLista2(TList *lisOBJ1, TList *lisOBJ2)
{
	// vari�veis locais
	TObject *obj;

	// prote��o
	if ((lisOBJ1 == NULL) || (lisOBJ2 == NULL))
		return (false);
	// verifica se o n�mero de objetos � igual nas duas listas
	if (lisOBJ1->Count != lisOBJ2->Count)
		return (false);
	// verifica se ambas listas est�o vazias
	if ((lisOBJ1->Count == 0) && (lisOBJ2->Count == 0))
		return (true);
	// verifica lisOBJ1 cont�m os mesmos objetos de lisOBJ2 SEM considerar a ordena��o
	return (Lista1ContemLista2(lisOBJ1, lisOBJ2));
}

// -----------------------------------------------------------------------------
EXPORT int __fastcall Lista1IntersecaoLista2(TList *lisOBJ1, TList *lisOBJ2)
{
	// vari�veis locais
	TObject *obj;
	int num_obj = 0;

	// prote��o
	if ((lisOBJ1 == NULL) || (lisOBJ2 == NULL))
		return (0);
	// prote��o: lista vazia
	if ((lisOBJ1->Count == 0) || (lisOBJ2->Count == 0))
		return (0);
	// conta n�mero de objetos de lisOBJ1 que est�o em lisOBJ2
	for (int n = 0; n < lisOBJ2->Count; n++)
	{
		obj = (TObject*)lisOBJ2->Items[n];
		if (lisOBJ1->IndexOf(obj) >= 0)
			num_obj++;
	}
	return (num_obj);
}
// -----------------------------------------------------------------------------
EXPORT AnsiString __fastcall RemoveCaracterInvalidoFilename(AnsiString filename)
{
	filename = StringReplace(filename, " ", "_",  TReplaceFlags() << rfReplaceAll);
	filename = StringReplace(filename, "<", "_",  TReplaceFlags() << rfReplaceAll);
	filename = StringReplace(filename, ">", "_",  TReplaceFlags() << rfReplaceAll);
	filename = StringReplace(filename, ":", "_",  TReplaceFlags() << rfReplaceAll);
	filename = StringReplace(filename, "\"", "_", TReplaceFlags() << rfReplaceAll);
	filename = StringReplace(filename, "\\", "_", TReplaceFlags() << rfReplaceAll);
	filename = StringReplace(filename, "/", "_",  TReplaceFlags() << rfReplaceAll);
	filename = StringReplace(filename, "|", "_",  TReplaceFlags() << rfReplaceAll);
	filename = StringReplace(filename, "?", "_",  TReplaceFlags() << rfReplaceAll);
	filename = StringReplace(filename, "*", "_",  TReplaceFlags() << rfReplaceAll);
	filename = StringReplace(filename, ".", "_",  TReplaceFlags() << rfReplaceAll);

	return filename;
}
// -----------------------------------------------------------------------------
EXPORT bool __fastcall RetaContemPonto(int x1, int y1, int x2, int y2, int x, int y)
{
	// vari�veis locais
	int dx, dy, xc, yc;
	double m;

	// calcula dx e dy
	dx = x2 - x1;
	dy = y2 - y1;
	// prote��o: verifica se (x1,y1) e (x2,y2) s�o iguais
	if ((dx == 0) && (dy == 0))
	{
		return ((x == x1) && (y == y1));
	}
	if (abs(dx) > abs(dy))
	{ // calcula declividade em rela��o � coordenada x
		m = double(dy) / double(dx);
		// define xc = x e calcula yc
		xc = x;
		yc = (m * (x - x1)) + y1;
	}
	else
	{ // calcula declividade em rela��o � coordenada y
		m = double(dx) / double(dy);
		// define yc = y e calcula xc
		yc = y;
		xc = (m * (y - y1)) + x1;
	}
	return ((xc == x) && (yc == y));
}

// -----------------------------------------------------------------------------
EXPORT void __fastcall RetiraLis1DeLis2(TList *lisOBJ1, TList *lisOBJ2)
{
	// vari�veis locais
	TObject *obj;

	// retira objetos presentes na lisOBJ1 da lista lisOBJ2
	for (int n = 0; n < lisOBJ1->Count; n++)
	{
		obj = (TObject*)lisOBJ1->Items[n];
		lisOBJ2->Remove(obj);
	}
}

// ---------------------------------------------------------------------------
EXPORT int __fastcall StrToInteger(AnsiString text)
{
	// vari�veis locais
	int valor = 0;

	// prote��o
	text = text.Trim();
	if (text.IsEmpty())
		return (0);
	try
	{ // converte AnsiString p/ int
		valor = text.ToInt();
	}
	catch (Exception &e)
	{ // nada a fazer
	}
	return (valor);
}

// -----------------------------------------------------------------------------
EXPORT bool __fastcall StrToInteger(AnsiString text, int &valor)
{
	// prote��o
	text = text.Trim();
	if (text.IsEmpty())
	{
		valor = 0;
		return (true);
	}
	try
	{ // converte AnsiString p/ int
		valor = text.ToInt();
	}
	catch (Exception &e)
	{
		return (false);
	}
	return (true);
}

/*
 //---------------------------------------------------------------------------
 EXPORT double __fastcall StrToDouble(AnsiString text)
 {
 //vari�veis locais
 double  valor;
 char    *pstr;
 byte    separador;
 byte    ponto = '.';

 //prote��o
 text = text.Trim();
 if (text.IsEmpty()) return(0.);
 //salva separador original
 separador = FormatSettings.DecimalSeparator;
 if (FormatSettings.DecimalSeparator != ponto)
 {//redefine separador decimal
 FormatSettings.DecimalSeparator = ponto;
 }
 //substitui caractere ',' por '.' no texto com valor double
 pstr    = text.c_str();
 if ((pstr = strchr(pstr, ',')) != NULL) *pstr = ponto;
 try{//prote��o
 valor    = text.ToDouble();         //converte AnsiString p/ double
 }catch(Exception &e)
 {
 valor = 0;
 }
 //restaura separador original
 if (FormatSettings.DecimalSeparator != separador)
 {//redefine separador decimal
 FormatSettings.DecimalSeparator = separador;
 }
 return(valor);
 }
 */
// ---------------------------------------------------------------------------
EXPORT double __fastcall StrToDouble(AnsiString text)
{
	// vari�veis locais
	double valor;

	// utiliza a fun��o StrToDouble que retorn indica��o de sucesso na convers�o
	if (!StrToDouble(text, valor))
		valor = 0;

	return (valor);
}
/*
// ---------------------------------------------------------------------------
EXPORT bool __fastcall StrToDouble(AnsiString text, double &valor)
{
	// vari�veis locais
	char *pstr;
	byte separador;
	byte ponto = '.';
	bool sucesso = true;

	// prote��o
	text = text.Trim();
	if (text.IsEmpty())
	{
		valor = 0.;
		return (true);
	}
	// salva separador original
	separador = FormatSettings.DecimalSeparator;
	if (FormatSettings.DecimalSeparator != ponto)
	{ // redefine separador decimal
		FormatSettings.DecimalSeparator = ponto;
	}
	// substitui caractere ',' por '.' no texto com valor double
	pstr = text.c_str();
	if ((pstr = strchr(pstr, ',')) != NULL)
		* pstr = ponto;
	try
	{ // prote��o
		valor = text.ToDouble(); // converte AnsiString p/ double
	}
	catch (Exception &e)
	{
		sucesso = false;
	}
	// restaura separador original
	if (FormatSettings.DecimalSeparator != separador)
	{ // redefine separador decimal
		FormatSettings.DecimalSeparator = separador;
	}
	return (sucesso);
}
*/
// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/15943923
EXPORT bool __fastcall StrToDouble(AnsiString text, double &valor)
{
	// vari�veis locais
	char *pstr;
	byte separador;
	bool sucesso = true;

	// prote��o
	text = text.Trim();
	if (text.IsEmpty())
	{
		valor = 0.;
		return (true);
	}
	// determina separador original
	separador = FormatSettings.DecimalSeparator;
	if (separador == '.')
	{ // substitui caractere ',' por '.' no texto com valor double
		pstr = text.c_str();
		if ((pstr = strchr(pstr, ',')) != NULL)
			* pstr = '.';
	}
	else if (separador == ',')
	{ // substitui caractere '.' por ',' no texto com valor double
		pstr = text.c_str();
		if ((pstr = strchr(pstr, '.')) != NULL)
			* pstr = ',';
	}
	try
	{ // converte AnsiString p/ double
		valor = text.ToDouble();
	}
//	catch (Exception &e)
//	catch(EConvertError &e)
	catch(...)
	{
		sucesso = false;
	}
	return (sucesso);
}

// ---------------------------------------------------------------------------
EXPORT TDate __fastcall StrToDoubleToData(AnsiString data)
{
	return (TDate(StrToDouble(data)));
}

// -----------------------------------------------------------------------------
EXPORT int __fastcall TempoDecorridoMS(TDateTime dttIni, TDateTime dttFim)
{
	// vari�veis locais
	TDateTime dif;
	unsigned short hour, min, sec, msec;
	int intTempoMS;

	dif = dttFim - dttIni;
	dif.DecodeTime(&hour, &min, &sec, &msec);
	intTempoMS = ((1000 * ((60 * ((60 * hour) + min)) + sec)) + msec);

	return (intTempoMS);
}

// ---------------------------------------------------------------------------
// eof
