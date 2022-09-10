![Cellular-Automata-Ubuntu-2022-09](https://user-images.githubusercontent.com/41707552/189493446-0fbbb17c-8c93-4498-aa03-a4b18e080355.gif)
<figcaption align = "center"><b>20x20 Amoeba Cellular Automata Rule</b></figcaption>
# CellularAutomata
A 3D Cellular Automata visualization using OpenGL based on Jason Rampe's [blog](https://softologyblog.wordpress.com/category/cellular-automata-2/). 

### Features
- Voxel rasterization using OpenGL, GLUT, GLEW
- Reliably performs up to ~125^3 voxels
- Custom CA rule parser for additional rules
- Handles Von Neumann and Moore Neighborhoods
- Handles Multi-State CA Rules (Game of Life is a 2 state CA Rule)
- Multithreaded CA Calculation
- Voxel coloring based on distance from center (Interpolation was based on (these logistic curves) [https://www.desmos.com/calculator/bfwup2kq8l])
- Lighting/shadows 
- Fuzzy String Matching for input menu

### Rules
- "445"
- "CLOUDS"
- "AMOEBA"
- "PULSE_WAVE"
- "von_Neumann_Builder"
- "Architecture"
- "Custom1"
- "Custom2"
- "Custom3"
- "Custom4"
- "DA_BRAIN"
- "vonN"
- "678"


### Controls
- 'i' -> rotate CA Cube about 'i' unit vector
- 'j' -> rotate CA Cube about 'j' unit vector
- 'k' -> rotate CA Cube about 'k' unit vector
- 'z' -> zoom in (finer tune)
- 'x' -> zoom in (coarser tune)
- 'p' -> pause animation
- 't' -> pause rotation
- 'a' -> translate by vec3(1,0,0)
- 'd' -> translate by vec3(-1,0,0)
- 'w' -> translate by vec3(0,1,0)
- 's' -> translate by vec3(0,-1,0)
- 'q' -> translate by vec3(0,0,1)
- 'e' -> translate by vec3(0,0,-1)
- 'esc' -> close window
