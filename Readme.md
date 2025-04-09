Aufgaben fuer naechstes mal:
- parser functionsfaehig machen
- atof aus fractol holen
- Wenn leere zeile in .rt ist, nicht abbrechen
- parsing tests machen (print_struct funktion oder so)
value = strtod(str, &endptr); schreiben


Step-by-Step Plan for Parsing
Here’s a clean, efficient plan to implement the parsing part of your miniRT project:

Step 1: Set Up the Environment
Goal: Ensure your project compiles and has the necessary dependencies.
Tasks:
Create a Makefile with rules: all, clean, fclean, re.
Link libft and MinilibX correctly (use -lm for math functions).
Test a basic main.c that opens a .rt file (e.g., open(argv[1], O_RDONLY)).


Step 2: Define Helper Functions
Goal: Build reusable tools for parsing.
Tasks:
Write a function to split a line into tokens: char **split_line(char *line) (use ft_split from libft with space as delimiter).
Write a function to validate and convert a string to a double: double parse_double(char *str, int *error).
Write a function to parse RGB colors: t_color parse_color(char *str, int *error) (e.g., "255,255,255" → t_color).
Write a function to parse a 3D vector: t_vec3 parse_vec3(char *str, int *error) (e.g., "0.0,1.0,0.0" → t_vec3).
Write an error handler: void exit_error(char *msg) (prints "Error\n" + message, then exits cleanly).


Step 3: Initialize the Scene
Goal: Prepare the t_scene struct for parsing.
Tasks:
Write t_scene init_scene(void):
Set ambient.is_set = 0, camera.is_set = 0.
Initialize arrays (lights, spheres, etc.) to NULL.
Set counters (light_count, etc.) to 0.
Test it in main to ensure no crashes.


Step 4: Parse the File Line-by-Line
Goal: Read the .rt file and identify element types.
Tasks:
Use get_next_line to read the file line-by-line.
For each line:
Skip empty lines (if (!line || !line[0]) continue;).
Split into tokens with split_line.
Check the first token (identifier: "A", "C", "L", "sp", "pl", "cy").
Call specific parsing functions based on the identifier (e.g., parse_ambient, parse_camera, etc.).


Step 5: Implement Element Parsing Functions
Goal: Convert each line into the appropriate struct.
Tasks:
t_ambient parse_ambient(char **tokens, t_scene *scene):
Check if scene->ambient.is_set == 1 (error if already set).
Validate token count (3: "A", ratio, color).
Parse ratio (parse_double), color (parse_color).
Set is_set = 1.
t_camera parse_camera(char **tokens, t_scene *scene):
Check if scene->camera.is_set == 1 (error if already set).
Validate token count (4: "C", position, orientation, FOV).
Parse position (parse_vec3), orientation (parse_vec3), FOV (parse_double).
Validate ranges (e.g., FOV in [0, 180]).
Set is_set = 1.
t_light parse_light(char **tokens, t_scene *scene):
Validate token count (4: "L", position, brightness, color).
Parse position (parse_vec3), brightness (parse_double), color (parse_color).
Allocate/reallocate scene->lights (e.g., scene->lights = realloc(scene->lights, sizeof(t_light) * (scene->light_count + 1))).
Add the new light and increment scene->light_count.
t_sphere parse_sphere(char **tokens, t_scene *scene):
Validate token count (4: "sp", center, diameter, color).
Parse center (parse_vec3), diameter (parse_double), color (parse_color).
Add to scene->spheres (realloc + increment sphere_count).
t_plane parse_plane(char **tokens, t_scene *scene):
Validate token count (4: "pl", position, orientation, color).
Parse position (parse_vec3), orientation (parse_vec3), color (parse_color).
Add to scene->planes.
t_cylinder parse_cylinder(char **tokens, t_scene *scene):
Validate token count (6: "cy", position, orientation, diameter, height, color).
Parse position (parse_vec3), orientation (parse_vec3), diameter (parse_double), height (parse_double), color (parse_color).
Add to scene->cylinders.


Step 6: Memory Management
Goal: Avoid leaks and handle errors gracefully.
Tasks:
Write a free_scene(t_scene *scene) function to free all allocated memory (lights, spheres, etc.).
Call it on error or program exit.
Test with valgrind to ensure no leaks.


Step 7: Validate and Test
Goal: Ensure parsing works with the example scene and edge cases.
Tasks:
Test with the provided example .rt file.
Add debug prints (e.g., print scene->ambient.ratio, scene->camera.fov, etc.) to verify parsing.
Test invalid cases (e.g., duplicate "A", malformed numbers) and check error messages.


Step 8: Integration
Goal: Prepare for the ray tracing part (not your focus, but good for handover).
Tasks:
Pass the parsed t_scene to your t_miniRT struct.
Ensure the struct is accessible for the rendering team.
