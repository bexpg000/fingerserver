#include "EmailPanel.h"
#include "GameMasterTool.h"

using namespace Lynx;

#define STATIC_BOX_RECV_NAME_LABEL(currCount, maxCount) (wxString::Format("�ռ���(%u/%u)", currCount, maxCount))
#define STATIC_BOX_CONTENTS_LABEL(currCount, maxCount) (wxString::Format("����(%u/%u)", currCount, maxCount))
#define STATIC_BOX_ITEM_LABEL(currCount, maxCount) (wxString::Format("����(%u/%u)", currCount, maxCount))

IMPLEMENT_CLASS(EmailPanel, wxPanel)

BEGIN_EVENT_TABLE(EmailPanel, wxPanel)
EVT_LIST_ITEM_RIGHT_CLICK(myID_EMAIL_PANEL_LIST_CTRL_ITEM, onRightClicked)
END_EVENT_TABLE()

EmailPanel::EmailPanel(wxWindow* parent, wxWindowID id, const wxPoint& pos,
    const wxSize& size, long style)
    : wxPanel(parent, id, pos, size, style)
{
     mStaticBoxRecvName = NULL;
     mTextCtrlRecvName = NULL;
     mStaticBoxContents = NULL;
     mTextCtrlContents = NULL;
     mStaticBoxItem = NULL;
     mBtnAddItem = NULL;
     mBtnDelAllItem = NULL;
     mListCtrlItem = NULL;
     mTextCtrlItemTemplateId = NULL;
     mTextCtrlItemCount = NULL;
     mTextCtrlRecord = NULL;
     mBtnSendEmail = NULL;
     mBtnClearRecord = NULL;
     mBtnReset = NULL;
     mOperateMenu = NULL;
}

EmailPanel::~EmailPanel()
{
    release();
}

bool
EmailPanel::initial()
{
    mOperateMenu = new wxMenu(wxT(""));
    mOperateMenu->Append(myID_EMAIL_PANEL_MENU_DEL_ITEM, wxT("ɾ������"));

    wxBoxSizer* topSizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(topSizer);

    // �ռ���
    mStaticBoxRecvName = new wxStaticBox(this, wxID_ANY, STATIC_BOX_RECV_NAME_LABEL(0, EMAIL_MULTICAST_MAX_COUNT));
    wxStaticBoxSizer* recvNameBoxSizer = new wxStaticBoxSizer(mStaticBoxRecvName, wxHORIZONTAL);
    topSizer->Add(recvNameBoxSizer, 0, wxGROW|wxALIGN_CENTRE|wxALL, 5);

    mTextCtrlRecvName = new wxTextCtrl(this, myID_EMAIL_PANEL_TEXT_CTRL_RECV_NAME, wxEmptyString,
        wxDefaultPosition, wxSize(600, 70), wxTE_MULTILINE | wxTE_WORDWRAP);
    recvNameBoxSizer->Add(mTextCtrlRecvName, 1, wxGROW|wxALL|wxALIGN_CENTRE, 5);

    // ����
    mStaticBoxContents = new wxStaticBox(this, wxID_ANY, STATIC_BOX_CONTENTS_LABEL(0, EMAIL_CONTENT_MAX_LENGTH));
    wxStaticBoxSizer* contentsSizer = new wxStaticBoxSizer(mStaticBoxContents, wxHORIZONTAL);
    topSizer->Add(contentsSizer, 0, wxGROW|wxALIGN_CENTRE|wxALL, 5);

    mTextCtrlContents = new wxTextCtrl(this, myID_EMAIL_PANEL_TEXT_CTRL_CONTENTS, wxEmptyString,
        wxDefaultPosition, wxSize(600, 70), wxTE_MULTILINE | wxTE_WORDWRAP);
    mTextCtrlContents->SetMaxLength(EMAIL_CONTENT_MAX_LENGTH);
    contentsSizer->Add(mTextCtrlContents, 1, wxGROW|wxALL|wxALIGN_CENTRE, 5);

    // ����
    mStaticBoxItem = new wxStaticBox(this, wxID_ANY, STATIC_BOX_ITEM_LABEL(0, EMAIL_ITEM_MAX_COUNT));
    wxStaticBoxSizer* itemSizer = new wxStaticBoxSizer(mStaticBoxItem, wxVERTICAL);
    topSizer->Add(itemSizer, 1, wxGROW|wxALIGN_CENTRE|wxALL, 5);

    wxBoxSizer* itemListCtrlSizer = new wxBoxSizer(wxHORIZONTAL);
    itemSizer->Add(itemListCtrlSizer, 1, wxGROW|wxALIGN_CENTRE|wxALL, 0);
    mListCtrlItem = createItemListCtrl();
    itemListCtrlSizer->Add(mListCtrlItem, 1, wxGROW|wxALL|wxALIGN_CENTRE, 5);
    
    wxBoxSizer* itemBtnSizer = new wxBoxSizer(wxHORIZONTAL);
    itemSizer->Add(itemBtnSizer, 0, wxGROW|wxALIGN_CENTRE|wxALL, 0);
    wxStaticText* itemTemplateIdText = new wxStaticText(this, wxID_STATIC, wxT("����ģ��ID"));
    mTextCtrlItemTemplateId = new wxTextCtrl(this, myID_EMAIL_DIALOG_TEXT_CTRL_ITEM_TEMPLATE_ID);
    wxStaticText* itemCountText = new wxStaticText(this, wxID_STATIC, wxT("��������"));
    mTextCtrlItemCount = new wxTextCtrl(this, myID_EMIAL_DIALOG_TEXT_CTRL_ITEM_COUNT);
    mBtnAddItem = new wxButton(this, myID_EMAIL_PANEL_ADD_ITEM, wxT("��ӵ���"));
    mBtnDelAllItem = new wxButton(this, myID_EMAIL_PANEL_DEL_ALL_ITEM, wxT("ɾ������"));

    mTextCtrlItemTemplateId->SetMaxLength(20);
    mTextCtrlItemCount->SetMaxLength(3);
    mBtnAddItem->Disable();
    mBtnDelAllItem->Disable();

    itemBtnSizer->Add(itemTemplateIdText, 0, wxGROW|wxALL|wxALIGN_CENTRE, 5);
    itemBtnSizer->Add(mTextCtrlItemTemplateId, 1, wxGROW|wxALL|wxALIGN_CENTRE, 5);
    itemBtnSizer->Add(itemCountText, 0, wxGROW|wxALL|wxALIGN_CENTRE, 5);
    itemBtnSizer->Add(mTextCtrlItemCount, 1, wxGROW|wxALL|wxALIGN_CENTRE, 5);
    itemBtnSizer->Add(mBtnAddItem, 0, wxGROW|wxALL|wxALIGN_CENTRE, 5);
    itemBtnSizer->Add(mBtnDelAllItem, 0, wxGROW|wxALL|wxALIGN_CENTRE, 5);

    // ��¼
    wxStaticBox* recordStaticBox = new wxStaticBox(this, wxID_ANY, wxT("��¼"));
    wxStaticBoxSizer* recordStaticBoxSizer = new wxStaticBoxSizer(recordStaticBox, wxHORIZONTAL);
    topSizer->Add(recordStaticBoxSizer, 1, wxGROW|wxALIGN_CENTRE|wxALL, 5);

    mTextCtrlRecord = new wxTextCtrl(this, myID_EMAIL_PANEL_TEXT_CTRL_RECORD, wxEmptyString,
        wxDefaultPosition, wxSize(600, 70), wxTR_DEFAULT_STYLE | wxNO_BORDER | wxTE_MULTILINE |
        wxTE_READONLY | wxTE_RICH);
    //mTextCtrlRecord->SetBackgroundColour(*wxBLACK);
    //mTextCtrlRecord->SetForegroundColour(*wxWHITE);
    recordStaticBoxSizer->Add(mTextCtrlRecord, 1, wxGROW|wxALL|wxALIGN_CENTRE, 5);

    // ��ť
    wxBoxSizer* btnBoxSizer = new wxBoxSizer(wxHORIZONTAL);
    topSizer->Add(btnBoxSizer, 0, wxGROW|wxALIGN_CENTRE|wxALL, 5);

    // ���Ͱ�ť
    mBtnSendEmail = new wxButton(this, myID_EMAIL_PANEL_SEND_EMAIL, wxT("�����ʼ�"));
    btnBoxSizer->Add(mBtnSendEmail, 0, wxALL|wxALIGN_CENTRE, 5);
    // ����UI��ť
    mBtnReset = new wxButton(this, myID_EMAIL_PANEL_RESET, wxT("����"));
    btnBoxSizer->Add(mBtnReset, 0, wxALL|wxALIGN_CENTRE, 5);
    // ��ռ�¼��ť
    mBtnClearRecord = new wxButton(this, myID_EMAIL_PANEL_CLEAR_RECORD, wxT("��ռ�¼"));
    btnBoxSizer->Add(mBtnClearRecord, 0, wxALL|wxALIGN_CENTRE, 5);

    topSizer->Fit(this);
    topSizer->SetSizeHints(this);

    // ��ʱ�����������
    long index = mListCtrlItem->InsertItem(mListCtrlItem->GetItemCount(), wxT(""));
    mListCtrlItem->SetItemPtrData(index, 123456789);
    mListCtrlItem->SetItem(index, ITEM_COL_TYPE_TEMPLATE_ID, wxString::Format("%u", 123456789));
    mListCtrlItem->SetItem(index, ITEM_COL_TYPE_ITEM_COUNT, wxString::Format("%u", 1));

    index = mListCtrlItem->InsertItem(mListCtrlItem->GetItemCount(), wxT(""));
    mListCtrlItem->SetItemPtrData(index, 123456789);
    mListCtrlItem->SetItem(index, ITEM_COL_TYPE_TEMPLATE_ID, wxString::Format("%u", 123456789));
    mListCtrlItem->SetItem(index, ITEM_COL_TYPE_ITEM_COUNT, wxString::Format("%u", 1));

    index = mListCtrlItem->InsertItem(mListCtrlItem->GetItemCount(), wxT(""));
    mListCtrlItem->SetItemPtrData(index, 123456789);
    mListCtrlItem->SetItem(index, ITEM_COL_TYPE_TEMPLATE_ID, wxString::Format("%u", 123456789));
    mListCtrlItem->SetItem(index, ITEM_COL_TYPE_ITEM_COUNT, wxString::Format("%u", 1));

    index = mListCtrlItem->InsertItem(mListCtrlItem->GetItemCount(), wxT(""));
    mListCtrlItem->SetItemPtrData(index, 123456789);
    mListCtrlItem->SetItem(index, ITEM_COL_TYPE_TEMPLATE_ID, wxString::Format("%u", 123456789));
    mListCtrlItem->SetItem(index, ITEM_COL_TYPE_ITEM_COUNT, wxString::Format("%u", 1));

    index = mListCtrlItem->InsertItem(mListCtrlItem->GetItemCount(), wxT(""));
    mListCtrlItem->SetItemPtrData(index, 123456789);
    mListCtrlItem->SetItem(index, ITEM_COL_TYPE_TEMPLATE_ID, wxString::Format("%u", 123456789));
    mListCtrlItem->SetItem(index, ITEM_COL_TYPE_ITEM_COUNT, wxString::Format("%u", 1));


    registerEvents();
    return true;
}

void
EmailPanel::release()
{
    if (mOperateMenu)
    {
        delete mOperateMenu;
        mOperateMenu = NULL;
    }
    
    deregisterEvents();
}

void
EmailPanel::registerEvents()
{

}

void
EmailPanel::deregisterEvents()
{

}

wxListCtrl*
EmailPanel::createItemListCtrl()
{
    wxListCtrl* listCtrl = new wxListCtrl(this, myID_EMAIL_PANEL_LIST_CTRL_ITEM, wxDefaultPosition, wxSize(600, 130), wxLC_REPORT);

    // ���ɲ���
    for (int index = 0; index < ITEM_COL_TYPE_MAX_COUNT; index++)
    {
        wxListItem itemCol;
        itemCol.SetText(getItemColTypeText(index));
        itemCol.SetWidth(300);
        listCtrl->InsertColumn(index, itemCol);
    }
    return listCtrl;
}

wxString 
EmailPanel::getItemColTypeText(int itemColType)
{
    switch (itemColType)
    {
    case ITEM_COL_TYPE_TEMPLATE_ID:
        return wxString(wxT("ģ��ID"));
    case ITEM_COL_TYPE_ITEM_COUNT:
        return wxString(wxT("����"));
    }

    return wxString(wxT("δ֪"));
}

void 
EmailPanel::onRightClicked(wxListEvent& ev)
{
    if (!mOperateMenu)
    {
        return;
    }

    if (!mListCtrlItem)
    {
        return;
    }

    if ((mListCtrlItem->GetWindowStyle() & wxLC_REPORT) && mListCtrlItem->GetSelectedItemCount() == 1)
    {
        wxPoint pt = ev.GetPoint();
        pt.y += mListCtrlItem->GetPosition().y;
        PopupMenu(mOperateMenu, pt);
    }
}

void 
EmailPanel::resetLayout()
{
    if (mTextCtrlRecvName)
    {
        mTextCtrlRecvName->Clear();
    }

    if (mTextCtrlContents)
    {
        mTextCtrlContents->Clear();
    }

    if (mListCtrlItem)
    {
        mListCtrlItem->DeleteAllItems();
    }

    if (mTextCtrlItemTemplateId)
    {
        mTextCtrlItemTemplateId->Clear();
    }

    if (mTextCtrlItemCount)
    {
        mTextCtrlItemCount->Clear();
    }

    if (mTextCtrlRecord)
    {
        mTextCtrlRecord->Clear();
    }

}

