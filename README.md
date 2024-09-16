
## Map Generator.

CLI map creator using Wave Function Collapse, a constraint-solving algorithm, to create 2D maps.

This is a simplified version of the map generator used in the game Brace the Swarm, and the maps exported from this creator can be used 'as is' in the game.
### How to use.

Clone repo to desired location.
```
git clone git@github.com:GlennNygard/map_generator.git
```

Open cloned repo then run the cmake build commands.
```
cmake -B build -S .
```
```
cmake --build build --target map_generator
```

This will fetch any required 3rd party libraries and build the project.

To run the map generator, just run the following command from the root directory:
```
./build/bin/map_generator
```

Some parameters, like the total count of maps created as well as their size, can be modified through the command line arguments.
```
./build/bin/map_generator --filename "MyCoolMap"
./build/bin/map_generator --count 5
./build/bin/map_generator --size medium
```

See a list of all possible arguments with:
```
./build/bin/map_generator --help
```


If the map size and map complexity is set too high, **the map generation might take a very long time or even fail**. Failure is an expected part of many constraint-solving algorithms, especially when using large input sets like here. In most cases the algorithm will automatically try again if it fails.


### Outputs.

The final export of the algorithm should look a little like the map presented below.

![Image 1](./images/bts_loadout_map_representation_showcase.jpg)

Keep in mind that exact node colours will be a bit different, and also that the _feel_ of the map can change quite a bit with the size. The exported thumbnails can be seen in the folder below after the program has run.

```
output/thumbnails
```

The maps themselves will be in the format .txt and stored in:
```
output/maps
```


There is no real way of visualising the map right now, so this is purely serves as a simple way to create maps for internal use (and explore the algorithm for the fun of it of course).

When visualised in the game Brace the Swarm, the generated maps would look something like this

![Image 2](./images/bts_classic_td_map_above.jpg)

BTS uses a more complex version of this algorithm for its internal map creation, but maps from this generator are still fully functional game maps.
In the future, I might get around to creating a public way of displaying and interacting with the maps generated as well.


### Current shortcomings.

There are some aspects that will heavily slow down how quickly the algorithm runs, such as the size of the input set (which currently accounts for almost 150 different elements) and map size. This is because reducing these will dramatically decrease the amount of calculations needed to solve a given part of the map, and also heavily reduce the likelihood of the generation process failing (and therefore needing to retry a given part).

Without reductions, expect the generation to take anywhere from 5-100+ seconds per map depending on hardware and map size.

While the memory layout and management during runtime is ok for the most part, this can still be further improved by reducing the space needed for various components and store them in a more efficient manner.

Some of these optimisations already exists in the 'game' version of this generator, and will hopefully make their way here as well at some point.

The final outputs are stored in a simple and readable .txt file. The benefits of this is that debugging is easy (if you know the file layout) and simple modifications are trivial to do. The main downside of this approach is that storing the map files will take quite a bit of space. It is therefore important that files are compressed before use.
