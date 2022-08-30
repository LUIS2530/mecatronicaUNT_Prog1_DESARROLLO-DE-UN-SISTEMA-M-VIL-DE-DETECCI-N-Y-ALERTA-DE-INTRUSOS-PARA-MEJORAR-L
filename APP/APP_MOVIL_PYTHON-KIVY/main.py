import kivy
kivy.require('2.0.0')
#kivy version 2.1.0
#kivymd version 0.104.2
#python 3.10.5
from kivy.uix.screenmanager import ScreenManager, Screen
from kivy.properties import ObjectProperty
from kivy.core.window import Window
from kivy.uix.boxlayout import BoxLayout

from kivymd.app import MDApp
from kivymd.uix.button import MDRectangleFlatButton, MDFillRoundFlatButton
from kivymd.uix.label import MDLabel
from kivymd.uix.toolbar import MDToolbar

#Window.size = (350,500) #tamaño app al iniciar main.py

class ContentNavigationDrawer(BoxLayout):
    nav_drawer = ObjectProperty()

class MainPage(ScreenManager):
    pass

class MainApp(MDApp):
    title = "Safe House"
    def go_home(self, *args):
        self.root.current = "primera_pagina"
    
    def change_style(self,señal,valor):
        if valor:
            self.theme_cls.theme_style ="Light"
        else:
            self.theme_cls.theme_style ="Dark"
    
    def build(self):
        self.theme_cls.theme_style ="Dark"
        self.theme_cls.primary_palette = 'Teal'
        return MainPage()

if __name__ == '__main__':
    MainApp().run()
