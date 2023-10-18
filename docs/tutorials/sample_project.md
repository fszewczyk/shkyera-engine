# Sample Project

Before continuing, make sure that you correctly undersood the concepts from the **Get Started!** guide.

Here, you can follow along the instructions to create different kinds of projects inside of Shkyera Engine. The projects are designed in a way to familiarize you with a multitude of concepts inside of the Shkyera Engine.

## Set up

Goal of this small project is to run a physics simulation of a rope and control it using our keyboard. Let's create an Empty Object, attach a `Script` component to it and create a file called `rope.py`. Drag the file into the component and open the file in your favorite text editor. Paste the boiler-plate code inside.

```{.py}
# Imports the boiler-plate object
from .lib.object import ShkyeraObject

class Object(ShkyeraObject):
    def setup(self):
        # This part will be executed at the beginning of the program

    def update(self):
        # This part will be executed at each frame (approximately 60 times a second)
```

## Physics

To use the physics engine, we need to import the appropriate module at the top of the file:

```{.py}
from .lib.physics import *
```

We want to be able to modify the length of the rope. Further, we want to specify the number of particles the rope should consist of. To do so, we make use of _public variables_.

```{.py}
class Object(ShkyeraObject):
    Length: float
    Particles: int
```

We also have to initialize the rope with the chosen settings. To do so, we modify the `setup` method.

```{.py}
def setup(self):
    self.rope = Rope(
        origin=self.get_position(), # Origin point of the rope should be equal to object's position
        alpha=0,                    # Initial angle of the rope
        length=self.Length,         # Length of the rope
        n_particles=self.Particles  # Number of particles that the rope is made from. Works best if between 10 and 50.
    )
```

Obviously, we want to be able to see the rope in our program. To do that, we need to draw it each time a frame is rendered. We modify the `update` method in the following way.

```{.py}
def update(self):
    # Draws the rope in the scene
    self.rope.draw()

    # Progresses the simulation by one timestep
    # If we did not call this, the rope would stay in place. Try for yourself :)
    self.rope.step()
```

Save the file, go back to Shkyera Engine and set `Length` to a value between 0.5 and 2.0 and the `Particles` to a value between 10 and 50. We are ready to launch the first version of our super fun game! Press `▷` to start the game.

<div align="center">
![Simple Rope](docs/img/basic_rope.gif)
</div>

Feel free to move the `X` and `Y` position in the `Transform` component. If you restart the program, your rope should originate at a different place. Remember the `origin=self.get_position()` part in the `setup` function? That's where it came from.

@note Be careful to not set the position to be too small (way below 0.0) or too large (way above 3.0), because the rope will be moved out of the rendering window.

## Keyboard Input

In order to move the rope around, we need to be able to read user input. We can do that using the module provided by Shkyera Engine. Add the following import at the top of your script.

```{.py}
from .lib.input import *
```

We modify the `update` method, such that we move our position based on the user input. We will move the rope by 0.02 units at each frame. The direction of movement depends on the pressed keys.

```{.py}
def update(self):
    if is_pressed('W'):
        self.position.y -= 0.02
    if is_pressed('S'):
        self.position.y += 0.02
    if is_pressed('A'):
        self.position.x -= 0.02
    if is_pressed('D'):
        self.position.x += 0.02

    self.rope.draw()
    self.rope.step()
```

@note Check out the [cheatsheet](https://fszewczyk.github.io/shkyera-engine/md_docs_tutorials_cheatsheet.html) for the full list of available keys.

Let's start it! After pressing `▷`, you should be able to move the rope around using W, A, S, D keys on your keyboard!

<div align="center">
![Moving Rope](docs/img/moving_rope.gif)
</div>

If you have encountered some problems, here is the whole contents of the script we wrote.

```{.py}
from .lib.object import ShkyeraObject

from .lib.physics import *
from .lib.input import *

class Object(ShkyeraObject):
    Length: float
    Particles: int

    def setup(self):
        self.rope = Rope(
            origin=self.get_position(),
            alpha=0,
            length=self.Length,
            n_particles=self.Particles
        )

    def update(self):
        if is_pressed('W'):
            self.position.y -= 0.02
        if is_pressed('S'):
            self.position.y += 0.02
        if is_pressed('A'):
            self.position.x -= 0.02
        if is_pressed('D'):
            self.position.x += 0.02

        self.rope.draw()
        self.rope.step()
```

Congratulations! Now you can read input from the keyboard and move the object around. Shkyera Engine is under constant development, so look out for new features coming out constantly. You can see the progress we are making [here](https://trello.com/b/mVROG8st/shkyera-engine).
