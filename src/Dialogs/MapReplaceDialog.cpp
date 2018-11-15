
// -----------------------------------------------------------------------------
// SLADE - It's a Doom Editor
// Copyright(C) 2008 - 2017 Simon Judd
//
// Email:       sirjuddington@gmail.com
// Web:         http://slade.mancubus.net
// Filename:    MapReplaceDialog.cpp
// Description: Dialog for 'Replace in Maps' functionality, allows to replace
//              all instances of a certain line special / thing type / etc in
//              all maps in an archive
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by the Free
// Software Foundation; either version 2 of the License, or (at your option)
// any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
// more details.
//
// You should have received a copy of the GNU General Public License along with
// this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA  02110 - 1301, USA.
// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------
//
// Includes
//
// -----------------------------------------------------------------------------
#include "Main.h"
#include "MapReplaceDialog.h"
#include "General/UI.h"
#include "MainEditor/ArchiveOperations.h"


// -----------------------------------------------------------------------------
//
// ThingTypeReplacePanel Class Functions
//
// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------
// ThingTypeReplacePanel class constructor
// -----------------------------------------------------------------------------
ThingTypeReplacePanel::ThingTypeReplacePanel(wxWindow* parent) : wxPanel(parent, -1)
{
	// Setup sizer
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	SetSizer(sizer);

	wxGridBagSizer* gbsizer = new wxGridBagSizer(UI::pad(), UI::pad());
	sizer->AddStretchSpacer();
	sizer->Add(gbsizer, 0, wxALIGN_CENTER | wxALL, UI::padLarge());
	sizer->AddStretchSpacer();

	// From type
	gbsizer->Add(
		new wxStaticText(this, -1, "Replace Type:"),
		wxGBPosition(0, 0),
		wxDefaultSpan,
		wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT);
	spin_from_ = new wxSpinCtrl(
		this, -1, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS | wxTE_PROCESS_ENTER, 0, 999999);
	gbsizer->Add(spin_from_, wxGBPosition(0, 1), wxDefaultSpan, wxEXPAND);

	// To type
	gbsizer->Add(
		new wxStaticText(this, -1, "With Type:"),
		wxGBPosition(1, 0),
		wxDefaultSpan,
		wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT);
	spin_to_ = new wxSpinCtrl(
		this, -1, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS | wxTE_PROCESS_ENTER, 0, 999999);
	gbsizer->Add(spin_to_, wxGBPosition(1, 1), wxDefaultSpan, wxEXPAND);
}

// -----------------------------------------------------------------------------
// ThingTypeReplacePanel class destructor
// -----------------------------------------------------------------------------
ThingTypeReplacePanel::~ThingTypeReplacePanel() {}

// -----------------------------------------------------------------------------
// Performs replace using settings from the panel controls for [archive]
// -----------------------------------------------------------------------------
void ThingTypeReplacePanel::doReplace(Archive* archive)
{
	size_t count = ArchiveOperations::replaceThings(archive, spin_from_->GetValue(), spin_to_->GetValue());
	wxMessageBox(
		S_FMT("Replaced %d occurrences. See console log for more detailed information.", count), "Replace Things");
}


// -----------------------------------------------------------------------------
//
// SpecialReplacePanel Class Functions
//
// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------
// SpecialReplacePanel class constructor
// -----------------------------------------------------------------------------
SpecialReplacePanel::SpecialReplacePanel(wxWindow* parent) : wxPanel(parent, -1)
{
	// Setup sizer
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	SetSizer(sizer);

	wxGridBagSizer* gbsizer = new wxGridBagSizer(UI::pad(), UI::pad());
	sizer->AddStretchSpacer();
	sizer->Add(gbsizer, 0, wxALIGN_CENTER | wxALL, UI::padLarge());

	// From special
	gbsizer->Add(
		new wxStaticText(this, -1, "Replace Special:"),
		wxGBPosition(0, 0),
		wxDefaultSpan,
		wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT);
	spin_from_ = new wxSpinCtrl(this, -1, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 999999);
	gbsizer->Add(spin_from_, wxGBPosition(0, 1), wxDefaultSpan, wxEXPAND);

	// To special
	gbsizer->Add(
		new wxStaticText(this, -1, "With Special:"),
		wxGBPosition(1, 0),
		wxDefaultSpan,
		wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT);
	spin_to_ = new wxSpinCtrl(this, -1, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 999999);
	gbsizer->Add(spin_to_, wxGBPosition(1, 1), wxDefaultSpan, wxEXPAND);

	// Replace line specials
	cb_line_specials_ = new wxCheckBox(this, -1, "Replace Line Specials");
	gbsizer->Add(cb_line_specials_, wxGBPosition(0, 2), wxDefaultSpan, wxEXPAND);

	// Replace thing specials
	cb_thing_specials_ = new wxCheckBox(this, -1, "Replace Thing Specials");
	gbsizer->Add(cb_thing_specials_, wxGBPosition(1, 2), wxDefaultSpan, wxEXPAND);

	sizer->Add(
		new wxStaticLine(this, -1, wxDefaultPosition, wxDefaultSize, wxHORIZONTAL),
		0,
		wxEXPAND | wxLEFT | wxRIGHT,
		UI::pad());

	// Args
	gbsizer = new wxGridBagSizer(UI::pad(), UI::pad());
	sizer->Add(gbsizer, 0, wxALIGN_CENTER | wxALL, UI::padLarge());
	for (unsigned a = 0; a < 5; a++)
	{
		// Create controls
		cb_args_[a]        = new wxCheckBox(this, -1, S_FMT("Arg %d", a));
		spin_args_from_[a] = new wxSpinCtrl(
			this, -1, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS | wxTE_PROCESS_ENTER, 0, 255);
		spin_args_to_[a] = new wxSpinCtrl(
			this, -1, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS | wxTE_PROCESS_ENTER, 0, 255);

		// Add to grid
		gbsizer->Add(cb_args_[a], wxGBPosition(a, 0), wxDefaultSpan, wxEXPAND);
		gbsizer->Add(
			new wxStaticText(this, -1, "Replace:"),
			wxGBPosition(a, 1),
			wxDefaultSpan,
			wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT);
		gbsizer->Add(spin_args_from_[a], wxGBPosition(a, 2), wxDefaultSpan, wxEXPAND);
		gbsizer->Add(
			new wxStaticText(this, -1, "With:"),
			wxGBPosition(a, 3),
			wxDefaultSpan,
			wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT);
		gbsizer->Add(spin_args_to_[a], wxGBPosition(a, 4), wxDefaultSpan, wxEXPAND);
	}

	sizer->AddStretchSpacer();

	cb_line_specials_->SetValue(true);
}

// -----------------------------------------------------------------------------
// SpecialReplacePanel class destructor
// -----------------------------------------------------------------------------
SpecialReplacePanel::~SpecialReplacePanel() {}

// -----------------------------------------------------------------------------
// Performs replace using settings from the panel controls for [archive]
// -----------------------------------------------------------------------------
void SpecialReplacePanel::doReplace(Archive* archive)
{
	size_t count = ArchiveOperations::replaceSpecials(
		archive,
		spin_from_->GetValue(),
		spin_to_->GetValue(),
		cb_line_specials_->GetValue(),
		cb_thing_specials_->GetValue(),
		cb_args_[0]->GetValue(),
		spin_args_from_[0]->GetValue(),
		spin_args_to_[0]->GetValue(),
		cb_args_[1]->GetValue(),
		spin_args_from_[1]->GetValue(),
		spin_args_to_[1]->GetValue(),
		cb_args_[2]->GetValue(),
		spin_args_from_[2]->GetValue(),
		spin_args_to_[2]->GetValue(),
		cb_args_[3]->GetValue(),
		spin_args_from_[3]->GetValue(),
		spin_args_to_[3]->GetValue(),
		cb_args_[4]->GetValue(),
		spin_args_from_[4]->GetValue(),
		spin_args_to_[4]->GetValue());

	wxMessageBox(
		S_FMT("Replaced %d occurrences. See console log for more detailed information.", count), "Replace Specials");
}


// -----------------------------------------------------------------------------
//
// TextureReplacePanel Class Functions
//
// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------
// TextureReplacePanel class constructor
// -----------------------------------------------------------------------------
TextureReplacePanel::TextureReplacePanel(wxWindow* parent) : wxPanel(parent, -1)
{
	// Setup sizer
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	SetSizer(sizer);

	wxGridBagSizer* gbsizer = new wxGridBagSizer(UI::pad(), UI::pad());
	sizer->AddStretchSpacer();
	sizer->Add(gbsizer, 0, wxALIGN_CENTER | wxALL, UI::pad());

	// From texture
	gbsizer->Add(
		new wxStaticText(this, -1, "Replace Texture:"),
		wxGBPosition(0, 0),
		wxDefaultSpan,
		wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT);
	text_from_ = new wxTextCtrl(this, -1);
	gbsizer->Add(text_from_, wxGBPosition(0, 1), wxDefaultSpan, wxEXPAND);

	// To texture
	gbsizer->Add(
		new wxStaticText(this, -1, "With Texture:"),
		wxGBPosition(1, 0),
		wxDefaultSpan,
		wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT);
	text_to_ = new wxTextCtrl(this, -1);
	gbsizer->Add(text_to_, wxGBPosition(1, 1), wxDefaultSpan, wxEXPAND);

	sizer->Add(
		new wxStaticLine(this, -1, wxDefaultPosition, wxDefaultSize, wxHORIZONTAL), 0, wxEXPAND | wxALL, UI::pad());

	gbsizer = new wxGridBagSizer(UI::pad(), UI::pad());
	sizer->Add(gbsizer, 0, wxALIGN_CENTER | wxALL, UI::pad());

	// Upper
	cb_upper_ = new wxCheckBox(this, -1, "Upper Textures");
	gbsizer->Add(cb_upper_, wxGBPosition(0, 0), wxDefaultSpan, wxEXPAND);

	// Middle
	cb_middle_ = new wxCheckBox(this, -1, "Middle Textures");
	gbsizer->Add(cb_middle_, wxGBPosition(1, 0), wxDefaultSpan, wxEXPAND);

	// Lower
	cb_lower_ = new wxCheckBox(this, -1, "Lower Textures");
	gbsizer->Add(cb_lower_, wxGBPosition(2, 0), wxDefaultSpan, wxEXPAND);

	// Floors
	cb_floor_ = new wxCheckBox(this, -1, "Floor Textures");
	gbsizer->Add(cb_floor_, wxGBPosition(0, 1), wxDefaultSpan, wxEXPAND);

	// Ceilings
	cb_ceiling_ = new wxCheckBox(this, -1, "Ceiling Textures");
	gbsizer->Add(cb_ceiling_, wxGBPosition(1, 1), wxDefaultSpan, wxEXPAND);

	sizer->AddStretchSpacer();
}

// -----------------------------------------------------------------------------
// TextureReplacePanel class destructor
// -----------------------------------------------------------------------------
TextureReplacePanel::~TextureReplacePanel() {}

// -----------------------------------------------------------------------------
// Performs replace using settings from the panel controls for [archive]
// -----------------------------------------------------------------------------
void TextureReplacePanel::doReplace(Archive* archive)
{
	size_t count = ArchiveOperations::replaceTextures(
		archive,
		text_from_->GetValue(),
		text_to_->GetValue(),
		cb_floor_->GetValue(),
		cb_ceiling_->GetValue(),
		cb_lower_->GetValue(),
		cb_middle_->GetValue(),
		cb_upper_->GetValue());

	wxMessageBox(
		S_FMT("Replaced %d occurrences. See console log for more detailed information.", count), "Replace Textures");
}


// -----------------------------------------------------------------------------
//
// MapReplaceDialog Class Functions
//
// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------
// MapReplaceDialog class constructor
// -----------------------------------------------------------------------------
MapReplaceDialog::MapReplaceDialog(wxWindow* parent, Archive* archive) :
	wxDialog(parent, -1, "Replace In Maps", wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
{
	// Init variables
	archive_ = archive;

	// Setup sizer
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	SetSizer(sizer);

	// Add tabs
	stc_tabs_ = STabCtrl::createControl(this);
	sizer->Add(stc_tabs_, 1, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, UI::padLarge());

	// Thing type tab
	panel_thing_ = new ThingTypeReplacePanel(stc_tabs_);
	stc_tabs_->AddPage(panel_thing_, "Thing Types");

	// Specials tab
	panel_special_ = new SpecialReplacePanel(stc_tabs_);
	stc_tabs_->AddPage(panel_special_, "Specials");

	// Textures tab
	panel_texture_ = new TextureReplacePanel(stc_tabs_);
	stc_tabs_->AddPage(panel_texture_, "Textures");

	// Dialog buttons
	btn_replace_     = new wxButton(this, -1, "Replace");
	btn_done_        = new wxButton(this, -1, "Close");
	wxBoxSizer* hbox = new wxBoxSizer(wxHORIZONTAL);
	hbox->AddStretchSpacer();
	hbox->Add(btn_replace_, 0, wxEXPAND | wxRIGHT, UI::pad());
	hbox->Add(btn_done_, 0, wxEXPAND, UI::pad());
	sizer->AddSpacer(UI::pad());
	sizer->Add(hbox, 0, wxLEFT | wxRIGHT | wxBOTTOM | wxEXPAND, UI::padLarge());

	// Setup dialog layout
	SetInitialSize(wxSize(-1, -1));
	Layout();
	Fit();
	SetMinSize(GetBestSize());
	CenterOnParent();

	// Bind events
	btn_done_->Bind(wxEVT_BUTTON, &MapReplaceDialog::onBtnDone, this);
	btn_replace_->Bind(wxEVT_BUTTON, &MapReplaceDialog::onBtnReplace, this);
}

// -----------------------------------------------------------------------------
// MapReplaceDialog class destructor
// -----------------------------------------------------------------------------
MapReplaceDialog::~MapReplaceDialog() {}


// -----------------------------------------------------------------------------
//
// MapReplaceDialog Class Events
//
// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------
// Called when the 'Done' button is clicked
// -----------------------------------------------------------------------------
void MapReplaceDialog::onBtnDone(wxCommandEvent& e)
{
	this->EndModal(wxID_OK);
}

// -----------------------------------------------------------------------------
// Called when the 'Replace' button is clicked
// -----------------------------------------------------------------------------
void MapReplaceDialog::onBtnReplace(wxCommandEvent& e)
{
	// Get current tab
	int current = stc_tabs_->GetSelection();

	// Thing types
	if (current == 0)
		panel_thing_->doReplace(archive_);

	// Specials
	else if (current == 1)
		panel_special_->doReplace(archive_);

	// Textures
	else if (current == 2)
		panel_texture_->doReplace(archive_);
}
