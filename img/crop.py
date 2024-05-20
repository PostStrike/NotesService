import os
from PIL import Image

def find_first_black_pixel_column(image, from_left=True):
    """
    Найти первую колонку, содержащую черный пиксель (RGB=(0,0,0)).
    
    :param image: Изображение (PIL Image)
    :param from_left: Начинать с левой стороны, если True, с правой стороны, если False
    :return: Индекс первой колонки с черным пикселем
    """
    width, height = image.size
    pixels = image.load()
    
    if from_left:
        range_func = range(width)
    else:
        range_func = range(width - 1, -1, -1)
    
    for x in range_func:
        for y in range(height):
            if pixels[x, y][3] != 0:  # Проверка альфа-канала
                return x
                
    return 0

def crop_to_first_black_pixel(image_path):
    """
    Обрезать изображение слева и справа до первой колонки с черным пикселем.
    
    :param image_path: Путь к изображению
    :return: Обрезанное изображение (PIL Image)
    """
    image = Image.open(image_path)
    
    left_bound = find_first_black_pixel_column(image, from_left=True)
    right_bound = find_first_black_pixel_column(image, from_left=False)
    
    print("{" + '"' + image_path + '"' + ", {" + str(left_bound) + ", " + str(right_bound) + "}" + "},")

for font_size in range(10, 81, 10):
# Папка с изображениями
    input_folder = 'symbol_images_' + str(font_size)

    # Проходим по всем файлам в папке
    for filename in os.listdir(input_folder):
        if filename.lower().endswith('.png') and not filename.lower().endswith('cursor_text.png'):
            # Полный путь к файлу
            input_path = os.path.join(input_folder, filename)
            crop_to_first_black_pixel(input_path)