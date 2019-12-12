#!/usr/bin/python3
#
# copyright by Oliver Dippel o.dippel@gmx.de 2019
#

import threading
import serial
import os
import copy
import sys
import glob
import datetime
import time
import argparse
import json
import gi
gi.require_version('Gtk', '3.0') 
from gi.repository import Gtk, GdkPixbuf, Gdk, Pango, Gio, GObject
from gi.repository.GdkPixbuf import Pixbuf, InterpType
import cairo
from io import StringIO

#
# #1;20.02;12.00;11.97;0.50;0.00
#


class MyGui(Gtk.Application):
	def __init__(self):
		Gtk.Application.__init__(self)
		if len(sys.argv) != 2:
			print("")
			print("USAGE:")
			print("	" + sys.argv[0] + " SERIAL_PORT")
			print("")
			exit(1)
		self.port = sys.argv[1]
		self.baud = 9600
		self.serial = serial.Serial(self.port, self.baud, timeout=10)
		thread = threading.Thread(target=self.read_from_port)
		thread.start()
   
	def read_from_port(self):
		self.running = True
		time.sleep(1.0)
		while self.running == True:
			try:
				reading = self.serial.readline().decode().strip()
				if reading.startswith("#0;") or reading.startswith("#1;"):
					parts = reading.split(";")
					timestamp = time.time()
					power = parts[0].strip("#")
					voltage_in = float(parts[1])
					voltage_set = float(parts[2])
					voltage_out = float(parts[3])
					ampere_set = float(parts[4])
					ampere_out = float(parts[5])
					if voltage_in >= 0.0 and voltage_in <= 50.0 and voltage_set >= 0.0 and voltage_set <= 50.0 and voltage_out >= 0.0 and voltage_out <= 50.0 and ampere_set >= 0.0 and ampere_set <= 5.0 and ampere_out >= 0.0 and ampere_out <= 5.0:
						self.timedata.append([timestamp, voltage_in, voltage_set, voltage_out, ampere_set, ampere_out])
						self.samples.set_markup("<span size='xx-large'>Samples: " + str(len(self.timedata)) + "</span>")
						self.voltage_in.set_markup("<span foreground='blue' size='xx-large'>Voltage-In: " + str(voltage_in) + "V</span>")
						self.voltage_out.set_markup("<span foreground='red' size='xx-large'>Voltage-Out: " + str(voltage_out) + "V</span>")
						self.ampere_out.set_markup("<span foreground='#ab00ab' size='xx-large'>Ampere-Out: " + str(ampere_out) + "A</span>")
						self.power.set_markup("<span foreground='#ab00ab' size='xx-large'>Power: " + str(power) + "</span>")
						self.volt.set_text(str(voltage_set))
						self.ampere.set_text(str(ampere_set))
						self.timeline.queue_draw()
					else:
						print("error")
						print(reading)
				else:
					print(reading)
					self.stat.set_text(reading)
			except:
				print("--")


	def do_activate(self):
		self.timedata = []
		self.window = Gtk.ApplicationWindow(application=self)
		self.window.connect("destroy", self.quit_callback)
		titlebar = self.create_titlebar()
		self.window.set_titlebar(titlebar)

		mainbox = Gtk.Box(orientation=Gtk.Orientation.VERTICAL)
		self.window.add(mainbox)

		mainbox2 = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL)
		mainbox.pack_start(mainbox2, True, True, 0)

		timevaluebox = Gtk.Box(orientation=Gtk.Orientation.VERTICAL)
		mainbox2.pack_start(timevaluebox, True, True, 0)

		timeline = self.create_timeline()
		timevaluebox.pack_start(timeline, True, True, 0)

		valuebox = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL)
		timevaluebox.add(valuebox)
		self.samples = Gtk.Label("--")
		valuebox.pack_start(self.samples, True, True, 0)
		self.voltage_in = Gtk.Label("--")
		valuebox.pack_start(self.voltage_in, True, True, 0)
		self.voltage_out = Gtk.Label("--")
		valuebox.pack_start(self.voltage_out, True, True, 0)
		self.ampere_out = Gtk.Label("--")
		valuebox.pack_start(self.ampere_out, True, True, 0)
		self.power = Gtk.Label("--")
		valuebox.pack_start(self.power, True, True, 0)

		buttonbox = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL)
		mainbox.add(buttonbox)
		btn_start = Gtk.Button.new_with_label("Start")
		btn_start.connect("clicked", self.btn_start)
		buttonbox.pack_start(btn_start, True, True, 0)
		btn_stop = Gtk.Button.new_with_label("Stop")
		btn_stop.connect("clicked", self.btn_stop)
		buttonbox.pack_start(btn_stop, True, True, 0)
		btn_on = Gtk.Button.new_with_label("On")
		btn_on.connect("clicked", self.btn_on)
		buttonbox.pack_start(btn_on, True, True, 0)
		btn_off = Gtk.Button.new_with_label("Off")
		btn_off.connect("clicked", self.btn_off)
		buttonbox.pack_start(btn_off, True, True, 0)
		btn_read = Gtk.Button.new_with_label("Read")
		btn_read.connect("clicked", self.btn_read)
		buttonbox.pack_start(btn_read, True, True, 0)

		settingbox = Gtk.Box(orientation=Gtk.Orientation.VERTICAL)
		mainbox2.add(settingbox)

		self.volt = self.add_setting(settingbox, "Volt", "", self.volt_set)
		self.ampere = self.add_setting(settingbox, "Ampere", "", self.ampere_set)

		self.window.show_all()

	def add_setting(self, settingbox, label, value, callback):
		box = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL)
		settingbox.add(box)
		label = Gtk.Label(label)
		box.pack_start(label, True, True, 0)
		entry = Gtk.Entry()
		entry.set_text(value)
		box.pack_start(entry, True, True, 0)
		btn_set = Gtk.Button.new_with_label("SET")
		btn_set.connect("clicked", callback)
		box.pack_start(btn_set, True, True, 0)
		return entry

	def ampere_set(self, button):
		data = "%1.02fA" % (float(self.load.get_text()), )
		self.stat.set_text(data)
		self.serial.write(data.encode())

	def volt_set(self, button):
		data = "LVP:%0.1f" % (float(self.lvp.get_text()), )
		self.stat.set_text(data)
		self.serial.write(data.encode())

	def btn_start(self, button):
		self.stat.set_text("start")
		self.serial.write("start".encode())

	def btn_stop(self, button):
		self.stat.set_text("stop")
		self.serial.write("stop".encode())

	def btn_on(self, button):
		self.stat.set_text("on")
		self.serial.write("on".encode())

	def btn_off(self, button):
		self.stat.set_text("off")
		self.serial.write("off".encode())

	def btn_read(self, button):
		self.stat.set_text("read")
		self.serial.write("read".encode())

	def quit_callback(self, action):
		self.running = False
		self.quit()

	def create_titlebar(self):
		hb = Gtk.HeaderBar()
		hb.set_show_close_button(True)
		hb.props.title = "DPS"
		box = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL)
		Gtk.StyleContext.add_class(box.get_style_context(), "linked")
		hb.pack_start(box)
		## Menuheader
		menubutton = Gtk.MenuButton.new()
		menumodel = Gio.Menu()
		menumodel.append("Export", "app.export")
		export_as = Gio.SimpleAction.new("export", None)
		export_as.connect("activate", self.export_as)
		self.add_action(export_as)
		menubutton.set_menu_model(menumodel)
		box.add(menubutton)
		return hb



	def timeline_draw_event(self, da, cairo_ctx):
		self.cw = self.timeline.get_allocation().width
		self.ch = self.timeline.get_allocation().height
		gx = 50
		gw = self.cw - gx - 50
		gh = self.ch - 10 - 10
		dl = len(self.timedata)
		if dl == 0:
			return


		#[timestamp, voltage_in, voltage_set, voltage_out, ampere_set, ampere_out]

		cairo_ctx.set_source_rgb(0.0, 0.0, 0.0)
		cairo_ctx.rectangle(gx - 1, 10 - 1, gw + 1, gh + 1)
		cairo_ctx.fill()

		# Grig
		cairo_ctx.set_source_rgb(0.5, 0.5, 0.5)
		cairo_ctx.set_line_width(0.5)
		for n in range(0, 50, 5):
			y = self.ch - 10 - int(n * gh / 50)
			cairo_ctx.new_path()
			cairo_ctx.move_to(gx, y)
			cairo_ctx.line_to(gx + gw, y)
			cairo_ctx.stroke()


		# voltage
		cairo_ctx.set_line_width(1.0)
		cairo_ctx.set_source_rgb(0.0, 0.0, 1.0)
		for n in range(0, 30 + 5, 5):
			y = self.ch - 10 - int(n * gh / 30)
			cairo_ctx.move_to(gx + gw + 10, y + 3)
			cairo_ctx.show_text(str(n) + "V")
			cairo_ctx.new_path()
			cairo_ctx.move_to(gx + gw, y)
			cairo_ctx.line_to(gx + gw + 5, y)
			cairo_ctx.stroke()
		cairo_ctx.new_path()
		tn = 1;
		for data in self.timedata:
			x = gx + tn * gw / dl
			y = self.ch - 10 - int(data[1] * gh / 30.0)
			if  tn == 1:
				cairo_ctx.move_to(gx, y)
			cairo_ctx.line_to(x, y)
			tn = tn + 1
		cairo_ctx.stroke()
		cairo_ctx.set_source_rgb(1.0, 0.0, 0.0)
		cairo_ctx.new_path()
		tn = 1;
		for data in self.timedata:
			x = gx + tn * gw / dl
			y = self.ch - 10 - int(data[2] * gh / 30.0)
			if  tn == 1:
				cairo_ctx.move_to(gx, y)
			cairo_ctx.line_to(x, y)
			tn = tn + 1
		cairo_ctx.stroke()
		cairo_ctx.set_source_rgb(0.0, 1.0, 0.0)
		cairo_ctx.new_path()
		tn = 1;
		for data in self.timedata:
			x = gx + tn * gw / dl
			y = self.ch - 10 - int(data[3] * gh / 30.0)
			if  tn == 1:
				cairo_ctx.move_to(gx, y)
			cairo_ctx.line_to(x, y)
			tn = tn + 1
		cairo_ctx.stroke()


		# ampere
		cairo_ctx.set_line_width(1.0)
		cairo_ctx.set_source_rgb(1.0, 1.0, 1.0)
		for n in range(0, 5 + 1, 1):
			y = self.ch - 10 - int(n * gh / 5)
			cairo_ctx.move_to(20, y + 3)
			cairo_ctx.show_text(str(n) + "A")
			cairo_ctx.new_path()
			cairo_ctx.move_to(gx - 10, y)
			cairo_ctx.line_to(gx - 5, y)
			cairo_ctx.stroke()
		cairo_ctx.new_path()
		tn = 1;
		for data in self.timedata:
			x = gx + tn * gw / dl
			y = self.ch - 10 - int(data[4] * gh / 5.0)
			if  tn == 1:
				cairo_ctx.move_to(gx, y)
			cairo_ctx.line_to(x, y)
			tn = tn + 1
		cairo_ctx.stroke()
		cairo_ctx.set_source_rgb(1.0, 1.0, 0.0)
		cairo_ctx.new_path()
		tn = 1;
		for data in self.timedata:
			x = gx + tn * gw / dl
			y = self.ch - 10 - int(data[5] * gh / 5.0)
			if  tn == 1:
				cairo_ctx.move_to(gx, y)
			cairo_ctx.line_to(x, y)
			tn = tn + 1
		cairo_ctx.stroke()


		cairo_ctx.set_source_rgb(0.0, 0.0, 0.0)
		cairo_ctx.rectangle(gx - 1, 10 - 1, gw + 1, gh + 1)
		cairo_ctx.stroke()

	def timeline_configure_event(self, da, event):
		allocation = da.get_allocation()
		self.surface = da.get_window().create_similar_surface(cairo.CONTENT_COLOR, allocation.width, allocation.height)
		cairo_ctx = cairo.Context(self.surface)
		cairo_ctx.set_source_rgb(1, 1, 1)
		cairo_ctx.paint()
		return True

	def create_timeline(self):
		self.timeline = Gtk.DrawingArea()
		self.timeline.set_size_request(800, 200)
		self.timeline.add_events(Gdk.EventMask.EXPOSURE_MASK | Gdk.EventMask.BUTTON_RELEASE_MASK | Gdk.EventMask.BUTTON_PRESS_MASK|Gdk.EventMask.POINTER_MOTION_MASK|Gdk.EventMask.SCROLL_MASK)
		self.timeline.connect('draw', self.timeline_draw_event)
		self.timeline.connect('configure-event', self.timeline_configure_event)
#		self.timeline.connect('button-press-event', self.timeline_clicked)
		return self.timeline

	def export_as(self, action, parameter):
		dialog = Gtk.FileChooserDialog("Please choose a file", self.window, Gtk.FileChooserAction.SAVE, (Gtk.STOCK_CANCEL, Gtk.ResponseType.CANCEL, Gtk.STOCK_SAVE, Gtk.ResponseType.OK))
		filter_xmeml = Gtk.FileFilter()
		filter_xmeml.set_name("CSV-Data")
		filter_xmeml.add_pattern("*.csv")
		dialog.add_filter(filter_xmeml)
		filter_any = Gtk.FileFilter()
		filter_any.set_name("Any files")
		filter_any.add_pattern("*")
		dialog.add_filter(filter_any)
		response = dialog.run()
		if response == Gtk.ResponseType.OK:
			print("CSV-Data save to " + dialog.get_filename())
			filename = dialog.get_filename()
			if not "." in filename:
				filename += ".csv"
			file = open(filename, "w")
			for data in self.timedata:
				line = ""
				for part in data:
					line += str(part) + ";"
				line = line.strip(";")
				file.write(line + "\r\n")
			file.close()
		dialog.destroy()



app = MyGui()

exit_status = app.run()
sys.exit(exit_status)

