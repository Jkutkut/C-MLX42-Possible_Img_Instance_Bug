#include "./MLX42/include/MLX42/MLX42.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <memory.h>

#define IMG_SIZE 64

#define MAP_WIDTH 6
#define MAP_HEIGHT 3

static mlx_image_t	*load_png(mlx_t *mlx, char *path)
{
	mlx_image_t		*img;
	mlx_texture_t	*texture;

	texture = mlx_load_png(path);
	img = mlx_texture_to_image(mlx, texture);
	mlx_delete_texture(texture);
	return (img);
}

static void	show(mlx_t *mlx, mlx_image_t *cat, mlx_image_t *wall, char **map)
{
	for (int i = 0, j; i < MAP_HEIGHT; i++) {
		for (j = 0; j < MAP_WIDTH; j++) {
			if (map[i][j] == '1')
			{
				printf("1");
				mlx_image_to_window(mlx, cat, j * 64, i * 64);
				// mlx_image_to_window(mlx, wall, j * 64, i * 64);
			}
			else if (map[i][j] == '0')
			{
				printf("0");
				// mlx_image_to_window(mlx, cat, j * 64, i * 64);
				mlx_image_to_window(mlx, wall, j * 64, i * 64);
			}
		}
		printf("\n");
	}
}

static void	analize_bug(mlx_image_t *cat, mlx_image_t *wall)
{
	printf("\nAnalize_bug\n\n");
	for (int i = 0, j; i < cat->count; i++) {
		for (j = 0; j < wall->count; j++) {
			if (&(cat->instances[i]) == &(wall->instances[j])) {
				printf("Memory collision: %x\n", &(cat->instances[i]));
				printf("Cat->instance[%d] = Wall->instance[%d]\n", i, j);
			}
		}
	}
}

int32_t	main(void)
{
	mlx_t		*mlx;
	mlx_image_t	*cat;
	mlx_image_t	*wall;
	char		*map[MAP_HEIGHT];

	mlx = mlx_init(IMG_SIZE * MAP_WIDTH, IMG_SIZE * MAP_HEIGHT, "MLX42 bug?", false);
	if (!mlx)
		exit(EXIT_FAILURE);
	cat = load_png(mlx, "res/cat.png");
	wall = load_png(mlx, "res/wall.png");
	map[0] = "111111";
	map[1] = "100001";
	map[2] = "111111";
	analize_bug(cat, wall);
	show(mlx, cat, wall, map);
	analize_bug(cat, wall);
	mlx_loop(mlx);
	return (EXIT_SUCCESS);
}
