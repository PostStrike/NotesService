from PIL import Image, ImageDraw, ImageFont
import string
import os

font_path = 'Latinia-Normal.ttf'

for font_size in range(10, 81, 10):
    if not os.path.exists("symbol_images_" + str(font_size)):
        os.makedirs("symbol_images_" + str(font_size))

    font = ImageFont.truetype(font_path, font_size)

    for char in string.printable:
        image = Image.new('RGB', (font_size, int(font_size + font_size * 0.4)), color='white')
        draw = ImageDraw.Draw(image)
        
        draw.text((0, 0), char, fill='black', font=font)
        
        image.save(os.path.join("symbol_images_" + str(font_size), f"{ord(char)}.png"))

    print("Изображения успешно созданы.")