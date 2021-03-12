// ---------------------------------------------------------------------------
#ifndef VTEstiloComentarioH
#define VTEstiloComentarioH

// ---------------------------------------------------------------------------
#include <Classes.hpp>
#include "TEstilo.h"

// ---------------------------------------------------------------------------
// Essa classe é meio provisória  pois hoje o texto não tem seu proprio estilo
// e a o comentario é rpaticametne um retangulo
// ---------------------------------------------------------------------------
class VTEstiloComentario : public TEstilo
{
public: // property
	__property int        BordaTipo      	= {read=bordaTipo,        	write=bordaTipo};
	__property int        BordaEspessura 	= {read=bordaEspessura,   	write=bordaEspessura};
	__property TColor     Bordacor	   		= {read=bordaCor,   		write=bordaCor};
	__property TColor     FundoCor 	     	= {read=fundoCor,   		write=fundoCor};
	__property bool       FundoTransparente = {read=fundoTransparente,  write=fundoTransparente};
	__property int 		  FontSize 			= {read=fontSize, 			write=fontSize};
	__property AnsiString FontName 			= {read=fontName, 			write=fontName};
	__property TColor	  FontColor			= {read=fontColor, 			write=fontColor};
	__property bool 	  FontBold 			= {read=fontBold, 			write=fontBold};
	__property bool 	  FontItalic 		= {read=fontItalic, 		write=fontItalic};
	__property bool 	  FontUnderline 	= {read=fontUnderline, 		write=fontUnderline};
	__property bool 	  FontStrikeOut 	= {read=fontStrikeOut, 		write=fontStrikeOut};

public:
			__fastcall VTEstiloComentario(void){};
	virtual __fastcall ~VTEstiloComentario(void){};

protected: // dados acessados via property
	AnsiString 	fontName;
	int 		fontSize;
	TColor 		fontColor;
	bool 		fontBold;
	bool 		fontItalic;
	bool 		fontUnderline;
	bool 		fontStrikeOut;
	int 		bordaTipo;
	int 		bordaEspessura;
	TColor 		bordaCor;
	TColor 		fundoCor;
	bool 		fundoTransparente;
};

#endif
// ---------------------------------------------------------------------------
// eof
