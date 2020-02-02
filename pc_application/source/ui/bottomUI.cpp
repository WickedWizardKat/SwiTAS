#include "bottomUI.hpp"

<<<<<<< HEAD
void JoystickCanvas::draw(wxDC* dc) {
	// Do thing
=======
JoystickCanvas::JoystickCanvas(wxFrame* parent)
	: wxGLCanvas(parent, wxID_ANY, NULL, wxDefaultPosition, wxDefaultSize, 0) {
	// Initialize base class
	co   = new wxGLContext((wxGLCanvas*)this);
	init = false;
}

// clang-format off
BEGIN_EVENT_TABLE(JoystickCanvas, wxGLCanvas)
	EVT_IDLE(JoystickCanvas::OnIdle)
	//EVT_SIZE(JoystickCanvas::OnResize)
END_EVENT_TABLE()
// clang-format on

// Override default signal handler:
void JoystickCanvas::draw() {
	// Use nanovg to draw a circle
	// SetCurrent sets the GL context
	// Now can use nanovg
	SetCurrent(*co);
	/*
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, (GLint)200, (GLint)200);
	glColor3f(1.0, c_, c_);

	glBegin(GL_POLYGON);
	glVertex3f(-0.5, -0.5, 5 * cos(rotate_));
	glVertex3f(-0.5, 0.5, 5 * cos(rotate_));
	glVertex3f(0.5, 0.5, -5 * cos(rotate_));
	glVertex3f(0.5, -0.5, -5 * cos(rotate_));
	glEnd();
	*/
	// Render
	SwapBuffers();
}

void JoystickCanvas::OnIdle(wxIdleEvent& event) {
	if(!init) {
		SetupViewport();
		init = true;
	}
	// Draw
	draw();
	// Dunno what this does
	event.RequestMore();
}

/*
void JoystickCanvas::OnResize(wxIdleEvent& event) {
	SetupViewport();
	Refresh();
}
*/

void JoystickCanvas::SetupViewport() {
	int x, y;
	GetSize(&x, &y);
	glViewport(0, 0, (GLint)x, (GLint)y);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (float)x / y, 0.1, 100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
>>>>>>> bf27387cf34d6d321956bfe23f51b7cccf5ad259
}

renderImageInGrid::renderImageInGrid(std::shared_ptr<wxBitmap> bitmap, Btn btn) {
	// Need users to know this is custom
	SetClientData((char*)"cus");
	theBitmap = bitmap;
	button    = btn;
}

void renderImageInGrid::setBitmap(std::shared_ptr<wxBitmap> bitmap) {
	theBitmap = bitmap;
}

void renderImageInGrid::Draw(wxGrid& grid, wxGridCellAttr& attr, wxDC& dc, const wxRect& rect, int row, int col, bool isSelected) {
	// Call base class ::Draw to clear the cell and draw the borders etc.
	wxGridCellRenderer::Draw(grid, attr, dc, rect, row, col, isSelected);
	// Draw rect in the right place
	dc.DrawBitmap(*theBitmap, rect.GetTopLeft());
}

/*
bool BottomUI::onButtonPress(GdkEventButton* event, Btn button) {
	// This button has just been clicked, notify the dataProcess
	inputInstance->toggleButtonState(button);
	return true;
}
*/

BottomUI::BottomUI(wxFrame* parentFrame, std::shared_ptr<ButtonData> buttons, wxFlexGridSizer* theGrid, std::shared_ptr<DataProcessing> input) {
	// TODO set up joysticks
	buttonData = buttons;

	inputInstance = input;
	// Callback stuff
	inputInstance->setInputCallback(std::bind(&BottomUI::setIconState, this, std::placeholders::_1, std::placeholders::_2));

	horizontalBoxSizer = std::make_shared<wxBoxSizer>(wxHORIZONTAL);

	leftJoystickDrawer = std::make_shared<JoystickCanvas>();
	leftJoystickDrawer->setBackgroundColor(*wxWHITE);
	rightJoystickDrawer = std::make_shared<JoystickCanvas>();
	rightJoystickDrawer->setBackgroundColor(*wxWHITE);

	horizontalBoxSizer->Add(leftJoystickDrawer.get(), wxEXPAND | wxALL);
	horizontalBoxSizer->Add(rightJoystickDrawer.get(), wxEXPAND | wxALL);

	buttonGrid = std::make_shared<wxGrid>();

	// Handle grid clicking
	buttonGrid->Bind(wxEVT_GRID_CELL_LEFT_CLICK, &BottomUI::onGridClick, this);

	for(auto const& button : KeyLocs) {
		buttonGrid->SetCellRenderer(button.second.y, button.second.x, new renderImageInGrid(buttonData->buttonMapping[button.first]->offBitmapIcon, button.first));
		/*
		// Add the images (the pixbuf can and will be changed later)
		std::shared_ptr<Gtk::Image> image = std::make_shared<Gtk::Image>(buttonData->buttonMapping[button.first].offIcon);
		// Add the eventbox
		std::shared_ptr<Gtk::EventBox> eventBox = std::make_shared<Gtk::EventBox>();
		eventBox->add(*image);
		eventBox->set_events(Gdk::BUTTON_PRESS_MASK);
		eventBox->signal_button_press_event().connect(sigc::bind<Btn>(sigc::mem_fun(*this, &BottomUI::onButtonPress), button.first));

		images.insert(std::pair<Btn, std::pair<std::shared_ptr<Gtk::Image>, std::shared_ptr<Gtk::EventBox>>>(button.first, { image, eventBox }));

		// Designate the off image as the default
		buttonViewer.attach(*eventBox, button.second.x, button.second.y);
		*/
	}

	horizontalBoxSizer->Add(buttonGrid.get(), wxEXPAND | wxALL);

	// Just add it
	theGrid->Add(horizontalBoxSizer.get(), wxEXPAND | wxALL);
}

void BottomUI::onGridClick(wxGridEvent& event) {
	// https://forums.wxwidgets.org/viewtopic.php?t=21585
	long col = event.GetCol();
	long row = event.GetRow();

	wxGridCellRenderer* cellRenderer = buttonGrid->GetCellRenderer(row, col);

	if(strcmp((char*)cellRenderer->GetClientData(), "cus") == 0) {
		// This is a custom cell renderer
		// Toggle the button state via the cell renderer hopefully
		inputInstance->toggleButtonState(((renderImageInGrid*)cellRenderer)->getButton());
	}

	event.Skip();
}

void BottomUI::setIconState(Btn button, bool state) {
	Location location = KeyLocs[button];
	if(state) {
		// Set the image to the on image
		((renderImageInGrid*)buttonGrid->GetCellRenderer(location.y, location.x))->setBitmap(buttonData->buttonMapping[button]->onBitmapIcon);
	} else {
		// Set the image to the off image
		((renderImageInGrid*)buttonGrid->GetCellRenderer(location.y, location.x))->setBitmap(buttonData->buttonMapping[button]->offBitmapIcon);
	}
	buttonGrid->RefreshRect(buttonGrid->CellToRect(location.x, location.x));

	// Don't set value in input instance because it
	// Was the one that sent us here
}