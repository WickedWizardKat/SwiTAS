#pragma once

#include <gtkmm/grid.h>
#include <gtkmm/hvbox.h>
#include <gtkmm/menubar.h>
#include <gtkmm/window.h>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include "bottomUI.hpp"
#include "dataProcessing.hpp"
#include "helpers.hpp"
#include "sideUI.hpp"

class MainWindow : public Gtk::Window {
private:
	// Layout that contains all items, including the menubar
	Gtk::VBox mainLayout;
	// Grid where all the layout stuff is dealt with
	Gtk::Grid mainGrid;
	// Grid containing the left hand side widgets
	Gtk::Grid leftGrid;
	// Grid containing the bottom widgets
	Gtk::Grid bottomGrid;
	// Menu bar containing "open file", etc...
	Gtk::MenuBar menuBar;

	// Main settings variable
	rapidjson::Document mainSettings;

	// The pointers to the classes containing the uis
	SideUI* sideUI;
	BottomUI* bottomUI;
	// Pointer to the class containing important input stuff
	DataProcessing* dataProcessingInstance;

	// Override default signal handler:
	bool on_key_press_event(GdkEventKey* event) override {
		// Handle keyboard input
		// A GDK int holding the pressed buttons
		guint key = event->keyval;
		// Send to the dataProcessing instance
		dataProcessingInstance->handleKeyboardInput(key);
		return Gtk::Window::on_key_press_event(event);
	}

	void handlePreviousWindowTransform() {
		// Resize and maximize as needed
		// TODO
	}

public:
	MainWindow() {
		// UI instances
		sideUI   = new SideUI();
		bottomUI = new BottomUI();
		// Both UIs need this
		dataProcessingInstance = new DataProcessing();
		sideUI->setInputInstance(dataProcessingInstance);
		bottomUI->setInputInstance(dataProcessingInstance);
		// Add mainLayout to window
		add(mainLayout);
		// Add the top menubar
		addMenuBar();
		// Add left UI
		addLeftUI();
		// Add bottom UI
		addBottomUI();
		// Adding all the grids
		addGrids();
		// Override the keypress handler
		add_events(Gdk::KEY_PRESS_MASK);
		// Get the main settings
		Helpers::getGlobalSettings(&mainSettings);
		handlePreviousWindowTransform();
	};

	void addGrids() {
		// Add left grid spanning the whole left side
		mainGrid.attach(leftGrid, 0, 0, 1, 2);
		// Add bottom grid spanning the rest of the bottom
		mainGrid.attach(bottomGrid, 1, 1, 1, 1);
		// Shows all the items in this container
		mainGrid.show_all();
		// Add all of them to the current layout
		mainLayout.pack_start(mainGrid, Gtk::PACK_SHRINK);
	}

	void addMenuBar() {
		// https://www.lucidarme.me/gtkmm-example-13/
		// Add menubar to layout
		mainLayout.pack_start(menuBar, Gtk::PACK_SHRINK);
	}

	void addLeftUI() {
		sideUI->addToGrid(&leftGrid);
	}

	void addBottomUI() {
		bottomUI->addToGrid(&bottomGrid);
	}

	~MainWindow() {
		delete sideUI;
		delete bottomUI;
		delete dataProcessingInstance;
	}
};