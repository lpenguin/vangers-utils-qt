# Vangers Utilites
This project aims to give the ability to view, export/import of various Vangers resource files with a nice GUI.  
![screenshot main](/images/screenshot-main.png?raw=true "Screenshot")

## Supported formats
### Image
* BMP (*.bmp, *.bmo) - 8bit image or sequence of images without transparency.
* XBM (*.xbm) - 8bit image with transparency.
* PNG + meta.txt - 8bit PNG with embedded palette and meta info for editing purposes.

### Palette
* PAL (*.pal) - Vangers palette file
* PNG (*.png) - PNG encoded palette

#### Image Meta
Vangers image files contain auxilary information about image size, frames count, image offset, etc.
The number and type of parameter is specifica to the concrete image.
This meta info can be viewed and edited with .meta.txt file genereated with PNG export. See example below.
**It is not advised to change the type, order or add/delete parameters.**
```
OffsetX:uint32 = 607
OffsetY:uint32 = 64
BSizeX:uint32 = 800
BSizeY:uint32 = 600
SizeX:uint32 = 179
SizeY:uint32 = 234
Size:uint32 = 25152
```

### How to edit the exported PNG-8 file
1. Open vangers BMP or XBM in Vangers Utils
2. Export it to the PNG
3. Open the PNG in GIMP
4. Bring the Palettes dockable window: Windows -> Dockable windows -> Palettes
5. Look for the palette "Colormap of image #..."
6. Doubleclik on it to open the palette
7. Edit the file
8. Save it as PNG with default settings
9. Open the new PNG in the Vangers Utils (you might need to close the old PNG in the viewer)
10. Export it as BMP or XBM


#### Adding a custom palette
You can add a custom palette to the palettes list. Open a *.pal or a *.png (PNG Palette) file and click "Add to palettes button". 
The custom palette will appear at the end of the list with prefix `custom/`. Now you can open an image and switch the palette to the new custom one.
