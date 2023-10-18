# Cheatsheet

## Boilerplate

This piece of code is the basis of every script. Always copy it into your file before implementing your own functionality.

```{.py}
# Imports the boiler-plate object
from .lib.object import ShkyeraObject

class Object(ShkyeraObject):
    def setup(self):
        # This part will be executed at the beginning of the program
        pass

    def update(self):
        # This part will be executed at each frame (approximately 60 times a second)
        pass
```

## Shapes

To render basic shapes, you can use the `Shape` component. You can create a `Rectangle`, a `Circle` or a `Line`. Each of them can be edited directly in Shkyera Engine. Position of the shape is directly linked to the `Transform` component of an object.

![Shape Component](docs/img/shape_component.gif)

## Drawing

Make sure you imported the drawing module at the top of your file.

```{.py}
from .lib.drawer import *
```

Before each `update` of the game, the view is completely cleared. That means that you need to redraw objects in each of your `update` functions if you want them to stay on the screen. Note that the provided values are pixel coordinates. Take a look at the following example. It's rather straightfoward.

```{.py}
# Imports the boiler-plate object
from .lib.object import ShkyeraObject

from .lib.drawer import *

class Object(ShkyeraObject):
    def setup(self):
        # This part will be executed at the beginning of the program
        pass

    def update(self):
        # This part will be executed at each frame (approximately 60 times a second)

        # We can draw a rectangle!
        draw_rectangle(
            x=50,
            y=10,
            w=400,
            h=50
        )

        # A circle
        draw_circle(
            x=120,
            y=180,
            r=60
        )

        # And a line!
        draw_line(
            x1=300,
            y1=200,
            x2=380,
            y2=260
        )
```

Using the above script allows for creating scenes such as this one.

![](docs/img/drawing_demo.png)

For a more detailed reference regarding drawing, check out the [original reference](https://fszewczyk.github.io/shkyera-engine/namespaceshkyera_1_1lib_1_1drawer.html).

## Input

Import the `input` module at the top of your script.

```{.py}
from .lib.input import *
```

Use the [`is_pressed(KEY_CODE)`](https://fszewczyk.github.io/shkyera-engine/namespaceshkyera_1_1lib_1_1input.html) function to check if a certain key was pressed. The key code is a string containing the code of the pressed key. Full list of available key codes is shown below. This function returns True if and only if selected key is currently being pressed.

### Examples

```{.py}
is_pressed("W")
is_pressed("UpArrow")
is_pressed("3")
```

### Key Codes

| Button Description | Key Code              |
| ------------------ | --------------------- |
| Tab                | `"Tab"`               |
| Left Arrow         | `"LeftArrow"      `   |
| Right Arrow        | `"RightArrow"       ` |
| Up Arrow           | `"UpArrow"          ` |
| Down Arrow         | `"DownArrow"        ` |
| Page Up            | `"PageUp"           ` |
| Page Down          | `"PageDown"         ` |
| Home               | `"Home"             ` |
| End                | `"End"              ` |
| Insert             | `"Insert"           ` |
| Delete             | `"Delete"           ` |
| Backspace          | `"Backspace"        ` |
| Space              | `"Space"            ` |
| Enter              | `"Enter"            ` |
| Escape             | `"Escape"           ` |
| Left Ctrl          | `"LeftCtrl"         ` |
| Left Shift         | `"LeftShift"        ` |
| Left Alt           | `"LeftAlt"          ` |
| Left Super         | `"LeftSuper"        ` |
| Right Ctrl         | `"RightCtrl"        ` |
| Right Shift        | `"RightShift"       ` |
| Right Alt          | `"RightAlt"         ` |
| Right Super        | `"RightSuper"       ` |
| Menu               | `"Menu"             ` |
| 0                  | `"0"                ` |
| 1                  | `"1"                ` |
| 2                  | `"2"                ` |
| 3                  | `"3"                ` |
| 4                  | `"4"                ` |
| 5                  | `"5"                ` |
| 6                  | `"6"                ` |
| 7                  | `"7"                ` |
| 8                  | `"8"                ` |
| 9                  | `"9"                ` |
| A                  | `"A"                ` |
| B                  | `"B"                ` |
| C                  | `"C"                ` |
| D                  | `"D"                ` |
| E                  | `"E"                ` |
| F                  | `"F"                ` |
| G                  | `"G"                ` |
| H                  | `"H"                ` |
| I                  | `"I"                ` |
| J                  | `"J"                ` |
| K                  | `"K"                ` |
| L                  | `"L"                ` |
| M                  | `"M"                ` |
| N                  | `"N"                ` |
| O                  | `"O"                ` |
| P                  | `"P"                ` |
| Q                  | `"Q"                ` |
| R                  | `"R"                ` |
| S                  | `"S"                ` |
| T                  | `"T"                ` |
| U                  | `"U"                ` |
| V                  | `"V"                ` |
| W                  | `"W"                ` |
| X                  | `"X"                ` |
| Y                  | `"Y"                ` |
| Z                  | `"Z"                ` |
| F1                 | `"F1"               ` |
| F2                 | `"F2"               ` |
| F3                 | `"F3"               ` |
| F4                 | `"F4"               ` |
| F5                 | `"F5"               ` |
| F6                 | `"F6"               ` |
| F7                 | `"F7"               ` |
| F8                 | `"F8"               ` |
| F9                 | `"F9"               ` |
| F10                | `"F10"              ` |
| F11                | `"F11"              ` |
| F12                | `"F12"              ` |
| Apostrophe         | `"Apostrophe"       ` |
| Comma              | `"Comma"            ` |
| Minus              | `"Minus"            ` |
| Period             | `"Period"           ` |
| Slash              | `"Slash"            ` |
| Semicolon          | `"Semicolon"        ` |
| Equal              | `"Equal"            ` |
| Left Bracket       | `"LeftBracket"      ` |
| Backslash          | `"Backslash"        ` |
| Right Bracket      | `"RightBracket"     ` |
| Grave Accent       | `"GraveAccent"      ` |
| Caps Lock          | `"CapsLock"         ` |
| Scroll Lock        | `"ScrollLock"       ` |
| Num Lock           | `"NumLock"          ` |
| Print Screen       | `"PrintScreen"      ` |
| Pause              | `"Pause"            ` |
| Keypad 0           | `"Keypad0"          ` |
| Keypad 1           | `"Keypad1"          ` |
| Keypad 2           | `"Keypad2"          ` |
| Keypad 3           | `"Keypad3"          ` |
| Keypad 4           | `"Keypad4"          ` |
| Keypad 5           | `"Keypad5"          ` |
| Keypad 6           | `"Keypad6"          ` |
| Keypad 7           | `"Keypad7"          ` |
| Keypad 8           | `"Keypad8"          ` |
| Keypad 9           | `"Keypad9"          ` |
| Keypad Decimal     | `"KeypadDecimal"    ` |
| Keypad Divide      | `"KeypadDivide"     ` |
| Keypad Multiply    | `"KeypadMultiply"   ` |
| Keypad Subtract    | `"KeypadSubtract"   ` |
| Keypad Add         | `"KeypadAdd"        ` |
| Keypad Equal       | `"KeypadEqual"      ` |
| Keypad Enter       | `"KeypadEnter"      ` |
