#pragma once

#include "ext/rho/rhoruby.h"
#include "resource.h"

namespace rho {

class CRhoSignatureWindow;
class CRhoSignature
{
    static CRhoSignatureWindow* m_pSigWindow;
public:
    enum ESignature{esModal, esNonModal};

    class CParams
    {
	public:
		CParams(ESignature eType, rho_param* params)
        {
			m_params = rho_param_dup(params);
            m_eType = eType;
		}
        ~CParams()
        {
            rho_param_free(m_params);
        }

        const String& getFilePath();
        const String& getFileFormat();
        int   getPenColor();
        int   getBgColor();
        int   getPenWidth();
        boolean hasBorder();
        CRect getWndRect();

        ESignature m_eType;
		rho_param* m_params;
        String m_strFilePath, m_strFileFormat;
	};

    class CModalParams: public CParams 
    {
	public:
        CModalParams(const char* szCallback, rho_param* params) : CParams(esModal, params)
        {
            m_callback_url = szCallback ? szCallback : "";
		}

		String m_callback_url;
	};

    class CNonModalParams: public CParams 
    {
	public:
        CNonModalParams(boolean bVisible, rho_param* params) : CParams(esNonModal, params)
        {
            m_bVisible = bVisible;
		}

		boolean m_bVisible;
	};

public:
    CRhoSignature(void);
    ~CRhoSignature(void);

    static void takeSignature(CParams* params);
    static void showSignature(CParams* params);
    static void saveSignature(CParams* params);
    static const char* getCaptureFile();
    static void clearSignature(int );
    static void hideSignature();
};

class CRhoSignatureWindow : public CDialogImpl <CRhoSignatureWindow>
{
    struct CSigPoint
    {
   	    short m_xPos;
	    short m_yPos;
	    bool  m_bNewLine;	

        CSigPoint() : m_xPos(-1), m_yPos(-1), m_bNewLine(false){}
        CSigPoint( int x, int y, bool bNewLine ) : m_xPos(x), m_yPos(y), m_bNewLine(bNewLine){}
    };

    //bool        m_bSigning;
    //int         m_nCurPoint;
    CPoint      m_ptLast;
    bool        m_bOutOfSignature, m_bCapture;
	Vector<CSigPoint> m_vecPoints;
	bool		m_bDoVectors;

    void addNewPoint(int x, int y, bool bNewLine);
    void sendVectors();
    void drawLastStroke();

    CRhoSignature::CParams* m_pParams;
    HRESULT m_hResult;
    HWND m_hWndCommandBar;

    HRESULT saveBitmapToFileByImageFactory( HBITMAP hBitmap, LPCTSTR filename, LPCTSTR format);
    void drawSignature( CDC& oDC, CRect& rcDraw );
public:
    CRhoSignatureWindow(CRhoSignature::CParams* pParams) : m_pParams(pParams), m_hWndCommandBar(0), m_bDoVectors(false), m_bOutOfSignature(false), m_bCapture(false){}
    ~CRhoSignatureWindow(){ delete m_pParams; }
	
	enum { IDD = IDD_TAKE_SIGNATURE };

	BEGIN_MSG_MAP(CRhoSignatureWindow)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroyDialog)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
        COMMAND_ID_HANDLER(IDM_CLEAR, OnClearCommand)
        MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
        MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUp)
        MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
        MESSAGE_HANDLER(WM_PAINT, OnPaint)

	END_MSG_MAP()

public:
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnDestroyDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
    LRESULT OnClearCommand(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	LRESULT OnLButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnLButtonUp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnMouseMove(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

    HRESULT getLatestHResult(){ return m_hResult; }
    CRhoSignature::CParams& getParams(){return *m_pParams;}

    void saveImage();
    void clearImage();
};

}//namespace rho