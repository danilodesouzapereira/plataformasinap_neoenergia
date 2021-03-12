// ---------------------------------------------------------------------------
#ifndef VTEstiloTextoH
#define VTEstiloTextoH

// ---------------------------------------------------------------------------
#include <Classes.hpp>

// ---------------------------------------------------------------------------
class VTEstiloTexto : public VTEstilo
{
	public: // property
		__property int			FontSize 		= {read = fontSize, write = fontSize};
		__property AnsiString 	FontName 		= {read = fontName, write = fontName};
		__property int 			FontColor 		= {read = fontColor, write = fontColor};
		__property bool 		FontBold 		= {read = fontBold, write = fontBold};
		__property bool 		FontItalic 		= {read = fontItalic, write = fontItalic};
		__property bool 		FontUnderline 	= {read = fontUnderline, write = fontUnderline};
		__property bool 		FontStrikeOut 	= {read = fontStrikeOut, write = fontStrikeOut};

	public:
		__fastcall VTEstiloTexto(void);
		__fastcall ~VTEstiloTexto(void);
		virtual bool __fastcall IsEqual(VTEstilo &ref) = 0;

	protected: // dados acessados via property
		AnsiString fontName;
		int fontSize;
		int fontColor;
		bool fontBold;
		bool fontItalic;
		bool fontUnderline;
		bool fontStrikeOut;
};

#endif
// ---------------------------------------------------------------------------
// eof
