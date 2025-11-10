#include "../cub3d.h"

void init_enemies(t_data *data)
{
    int i;

    i = 0;
    data->enemy_count = 3;
    data->enemies = malloc(sizeof(t_enemy) * data->enemy_count);
    
    printf("Player starting at: (%.2f, %.2f)\n", data->player.curr_x, data->player.curr_y);
    

    data->enemies[0].x = data->player.curr_x + 3.0;
    data->enemies[0].y = data->player.curr_y;
    data->enemies[0].alive = true;
    
    data->enemies[1].x = data->player.curr_x + 2.0;
    data->enemies[1].y = data->player.curr_y;
    data->enemies[1].alive = true;

    data->enemies[2].x = data->player.curr_x - 3.0;
    data->enemies[2].y = data->player.curr_y;
    data->enemies[2].alive = true;
    
    mlx_texture_t *enemy_tex = mlx_load_png("resources/skeleton_w.png");
    while (i < data->enemy_count)
        data->enemies[i++].texture = enemy_tex;
}

void    calculate_enemy_distance(t_data *data, int i,  t_enemy_vars *vars)
{
    double dir_x;
    double dir_y;
    vars->dx = data->enemies[i].x - data->player.curr_x;
    vars->dy = data->enemies[i].y - data->player.curr_y;
    data->enemies[i].distance = sqrt(vars->dx * vars->dx + vars->dy * vars->dy);
    dir_x = -vars->dx / data->enemies[i].distance;
    dir_y = -vars->dy / data->enemies[i].distance;
    vars->new_x = data->enemies[i].x + dir_x * vars->move_speed;
     vars->new_y = data->enemies[i].y + dir_y * vars->move_speed;
}

void    check_if_can_move(t_data *data, int i, t_enemy_vars *vars)
{
    int j;
    double dx;
    double dy;

    j = 0;
    vars->can_move = 1;
    if (data->map[(int)vars->new_y][(int)vars->new_x] == '1')
        vars->can_move = 0;       

    while (j < data->enemy_count && vars->can_move)
    {
        if (j != i && data->enemies[j].alive)
        {
            dx = vars->new_x - data->enemies[j].x;
            dy = vars->new_y - data->enemies[j].y;
            if (sqrt(dx * dx + dy * dy) < 0.8)
                vars->can_move = 0;
        }
            j++;
    }
}

void update_enemies(t_data *data)
{
    t_enemy_vars *vars;

    vars = &data->enemy_vars;
    int i = 0;
    while (i < data->enemy_count)
    {
        if (data->enemies[i].alive)
        {
            calculate_enemy_distance(data, i, vars);
            vars->move_speed = 0.05;
            if (data->enemies[i].distance > 2.0)
            {
                check_if_can_move(data, i, vars);
                
                if (vars->can_move)
                {
                    data->enemies[i].x = vars->new_x;
                    data->enemies[i].y = vars->new_y;
                }
            }
        }
        i++;
    }
}

void sort_enemies(t_data *data)
{
    int i = 0;
    while (i < data->enemy_count - 1)
    {
        int j = 0;
        while (j < data->enemy_count - i - 1)
        {
            if (data->enemies[j].distance < data->enemies[j + 1].distance)
            {
                t_enemy temp = data->enemies[j];
                data->enemies[j] = data->enemies[j + 1];
                data->enemies[j + 1] = temp;
            }
            j++;
        }
        i++;
    }
}

void get_sprite_transform(t_data *data, t_enemy *enemy, double *transform_x, double *transform_y)
{
    double sprite_x = enemy->x - data->player.curr_x;
    double sprite_y = enemy->y - data->player.curr_y;
    double inv_det = 1.0 / (data->raycast.plane_x * data->raycast.dir_y -
                             data->raycast.dir_x * data->raycast.plane_y);
    
    *transform_x = inv_det * (data->raycast.dir_y * sprite_x -
                               data->raycast.dir_x * sprite_y);
    *transform_y = inv_det * (-data->raycast.plane_y * sprite_x +
                               data->raycast.plane_x * sprite_y);
}

void get_sprite_dimensions(double transform_x, double transform_y, int *screen_x, 
                           int *height)
{
    *screen_x = (int)((WIN_WIDTH / 2) * (1 + transform_x / transform_y));
    *height = abs((int)(WIN_HEIGHT / transform_y));
}

void get_draw_bounds(int sprite_screen_x, int sprite_size, int *start_x, int *end_x)
{
    *start_x = -sprite_size / 2 + sprite_screen_x;
    if (*start_x < 0)
        *start_x = 0;
    *end_x = sprite_size / 2 + sprite_screen_x;
    if (*end_x >= WIN_WIDTH)
        *end_x = WIN_WIDTH - 1;
}

void get_draw_bounds_y(int sprite_size, int *start_y, int *end_y)
{
    *start_y = -sprite_size / 2 + WIN_HEIGHT / 2;
    if (*start_y < 0)
        *start_y = 0;
    *end_y = sprite_size / 2 + WIN_HEIGHT / 2;
    if (*end_y >= WIN_HEIGHT)
        *end_y = WIN_HEIGHT - 1;
}

void draw_sprite_pixel(t_data *data, t_enemy *enemy, int tex_x, int tex_y, int stripe, int y)
{
    uint32_t *pixels;
    uint32_t color;
    
    if (tex_x >= 0 && tex_x < (int)enemy->texture->width &&
        tex_y >= 0 && tex_y < (int)enemy->texture->height)
    {
        pixels = (uint32_t *)enemy->texture->pixels;
        color = pixels[tex_y * enemy->texture->width + tex_x];
        if ((color & 0x000000FF) != 0)
            mlx_put_pixel(data->canvas, stripe, y, color);
    }
}

void draw_sprite_column(t_data *data, t_enemy *enemy, int stripe, t_sprite_data *sp)
{
    int tex_x;
    int y;
    int d;
    int tex_y;
    double stripe_offset;
    double actual_distance;
    
    tex_x = (int)((stripe - (-sp->sprite_width / 2 + sp->sprite_screen_x)) *
                  enemy->texture->width / sp->sprite_width);
    stripe_offset = (stripe - sp->sprite_screen_x) / (double)WIN_WIDTH;
    actual_distance = sp->transform_y / cos(atan(stripe_offset));
    if (actual_distance >= data->wall_distances[stripe])
        return;
    y = sp->draw_start_y;
    while (y < sp->draw_end_y)
    {
        d = y - WIN_HEIGHT / 2 + sp->sprite_height / 2;
        tex_y = d * enemy->texture->height / sp->sprite_height;
        draw_sprite_pixel(data, enemy, tex_x, tex_y, stripe, y);
        y++;
    }
}

void draw_enemy(t_data *data, t_enemy *enemy)
{
    double transform_x, transform_y;
    t_sprite_data sp;
    int draw_start_x, draw_end_x;
    int stripe;
    
    get_sprite_transform(data, enemy, &transform_x, &transform_y);
    if (transform_y <= 0)
        return;
    get_sprite_dimensions(transform_x, transform_y, &sp.sprite_screen_x, &sp.sprite_height);
    sp.sprite_width = sp.sprite_height;
    sp.transform_y = transform_y;
    get_draw_bounds(sp.sprite_screen_x, sp.sprite_width, &draw_start_x, &draw_end_x);
    get_draw_bounds_y(sp.sprite_height, &sp.draw_start_y, &sp.draw_end_y);
    stripe = draw_start_x;
    while (stripe < draw_end_x)
    {
        draw_sprite_column(data, enemy, stripe, &sp);
        stripe++;
    }
}