import os

from PySide6.QtCore import QFileSystemWatcher
from PySide6.QtWidgets import QApplication, QMainWindow, QLabel
from PySide6.QtGui import QPixmap, QColor, QImage

        
class Memory():
    def __init__(self, memory_path):
        self.data_memory = []
        self.memory_path = memory_path
        self.read_memory()
    
    def error(self):
        pass
    
    def read_memory(self):
        try:
            self.data_memory = []
            memory_fd = open(self.memory_path, "r")
            for line in memory_fd:
                line = line.split()
                self.data_memory.append(int(line[1], 16))
            memory_fd.close()
        except Exception as err:
            self.error()
        
class Screen(QMainWindow):
    def __init__(self, mem):                
        super().__init__()
        
        assert isinstance(mem, Memory)
        self.memory = mem
        self.width, self.height, self.size_factor = self.get_init_parameter(self.memory.data_memory)
        self.pixels = []
        
    
    def create_image(self):
        image = QImage(self.width, self.height, QImage.Format_RGB32)
        for x in range(self.width):
            for y in range(self.height):
                image.setPixelColor(x, y, QColor('white'))
        for px in self.pixels:
            image.setPixelColor(px.get('x'), px.get('y'), QColor('black'))
        return image
    
    def get_init_parameter(self, data_memory):
        height = data_memory[0] & 0x00000fff
        width = (data_memory[0] >> 12) & 0x00000fff
        size_factor = (data_memory[0] >> 24) & 0x0000000f
        if size_factor == 0:
            size_factor = 1
        return width, height, size_factor
    
    def get_px(self, data_memory):
        for px in data_memory[1:(self.width*self.height)+self.width+1]:
            px_parameter = {}
            px_parameter["y"] = px & 0x00000fff
            px_parameter["x"] = (px >> 12) & 0x00000fff
            self.pixels.append(px_parameter)
            
    def image_to_pixmap(self, image):
        pixmap = QPixmap.fromImage(image).scaled(self.width*self.size_factor, self.height*self.size_factor)
        return pixmap
        
    def show_pixmap(self, pixmap):
        label = QLabel(self)
        label.setPixmap(pixmap)
        self.setCentralWidget(label)
        
def on_memory_modified(memory_path, mem, screen):
    mem.read_memory()
    screen.get_px(mem.data_memory)
    image = screen.create_image()
    pixmap = screen.image_to_pixmap(image)
    screen.show_pixmap(pixmap)
    screen.show()
    
def main():
    app = QApplication()
    
    memory_path = os.path.join(os.getcwd(), "memory/.memory.bin")
    mem = Memory(memory_path)
    screen = Screen(mem)
    on_memory_modified(memory_path, mem, screen)
    watcher = QFileSystemWatcher([memory_path])
    watcher.fileChanged.connect(lambda: on_memory_modified(memory_path, mem, screen))
    
    app.exec()

if __name__ == '__main__':
    main()
