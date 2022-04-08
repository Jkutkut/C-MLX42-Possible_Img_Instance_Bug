#include "./MLX42/include/MLX42/MLX42.h"
#include "./MLX42/include/MLX42/MLX42_Int.h"
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
	int instancesId[MAP_HEIGHT][MAP_WIDTH];

	for (int i = 0, j; i < MAP_HEIGHT; i++) {
		for (j = 0; j < MAP_WIDTH; j++) {
			if (map[i][j] == '1')
			{
				printf("1");
				// instancesId[i][j] = mlx_image_to_window(mlx, cat, j * 64, i * 64);
				instancesId[i][j] = mlx_image_to_window(mlx, wall, j * 64, i * 64);
			}
			else if (map[i][j] == '0')
			{
				printf("0");
				instancesId[i][j] = mlx_image_to_window(mlx, cat, j * 64, i * 64);
				// instancesId[i][j] = mlx_image_to_window(mlx, wall, j * 64, i * 64);
			}
		}
		printf("\n");
	}
	printf("\n\nInstances id:\n");
	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			printf("%2d ", instancesId[i][j]);
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
				printf("Memory collision: %p\n", &(cat->instances[i]));
				printf("Cat->instance[%d] = Wall->instance[%d]\n", i, j);
			}
		}
	}
}

static void	analize_bug2(mlx_t *mlx, mlx_image_t *cat, mlx_image_t *wall)
{
	mlx_ctx_t		*ctx;
	mlx_list_t		*lst;
	draw_queue_t	*queue;

	printf("\nAnalize_bug2 - Render queue\n\n");
	ctx = mlx->context;
	lst = ctx->render_queue;
	while (lst) {
		queue = (draw_queue_t *)(lst->content);
		if (queue->image == cat) {
			printf("Cat\n");
		}
		if (queue->image == wall) {
			printf("Wall\n");
		}
		printf("  pos: (%02d, %02d),", queue->image->instances[queue->instanceid].x / 64, queue->image->instances[queue->instanceid].y / 64);
		printf("  instanceid: %02d/%02d", queue->instanceid, queue->image->count);
		printf("\n");
		lst = lst->next;
	}
}

static void	analize_bug3(mlx_image_t *cat, mlx_image_t *wall)
{
	printf("\nAnalize_bug3\n\n");
	printf("cat pixels : %p\n", cat->pixels);
	printf("wall pixels: %p\n", wall->pixels);
}

static void analize_bug4(mlx_image_t *cat, mlx_image_t *wall)
{
	printf("\nAnalize_bug4\n\n");
	printf("Images are equal: %d\n", memcmp(cat->pixels, wall->pixels, cat->width * cat->height * sizeof(uint8_t)));
}

static void	analize_bug5(mlx_t *mlx)
{
	mlx_ctx_t		*ctx;
	mlx_list_t		*lst;
	draw_queue_t	*queue;

	printf("\nAnalize_bug5 - Moving imgs\n\n");

	ctx = mlx->context;
	lst = ctx->render_queue;
	int i = 0;
	while (lst && i < 7) {
		lst = lst->next;
		i++;
	}

	queue = (draw_queue_t *)(lst->content);
	mlx_image_t *img = queue->image;
	// img->instances[queue->instanceid].y += 20;
	for (int i = 0; i < img->count; i++)
		img->instances[i].y += 20;
	printf("0s should be 20px lower\n");
}

static void hook(mlx_t *mlx) {
	// sleep(3);
	// mlx_close_window(mlx);
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
	analize_bug2(mlx, cat, wall);
	analize_bug3(cat, wall);
	analize_bug4(cat, wall);
	analize_bug5(mlx);
	mlx_loop_hook(mlx, &hook, mlx);
	mlx_loop(mlx);
	return (EXIT_SUCCESS);
}
