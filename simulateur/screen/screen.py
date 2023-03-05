import os
import time

from PySide6.QtCore import QFileSystemWatcher
from PySide6.QtWidgets import QApplication, QMainWindow, QLabel
from PySide6.QtGui import QPixmap, QColor, QImage, QPainter, QPen

        
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
        self.width, self.height, self.size_factor, self.grid_state = self.get_init_parameter(self.memory.data_memory)
        self.size_factor = 30
        self.pixels = []
    
    def update_image(self):
        image = QImage(self.width, self.height, QImage.Format_RGB32)
        for px in self.pixels:
            r = (px.get("color") >> 16) & 0x000000ff
            g = (px.get("color") >> 8) & 0x000000ff
            b = px.get("color") & 0x000000ff
            image.setPixelColor(px.get('x'), px.get('y'), QColor(r, g, b))
        return image
    
    def get_init_parameter(self, data_memory):
        width = data_memory[0] & 0x0000000f
        height = (data_memory[0] >> 4) & 0x0000000f
        size_factor = ((data_memory[0] >> 8) & 0x0000000f | ((data_memory[0] >> 12) & 0x0000000f << 4))
        grid_state = (data_memory[0] >> 16) & 0x0000000f
        return width, height, size_factor, grid_state
    
    def get_px(self, data_memory):
        for px in data_memory[1:(self.width*self.height)+self.width+1]:
            px_parameter = {}
            px_parameter["color"] = px & 0x00ffffff
            px_parameter["y"] = (px >> 24) & 0x0000000f
            px_parameter["x"] = (px >> 28) & 0x0000000f
            self.pixels.append(px_parameter)
            
    def image_to_pixmap(self, image):
        pixmap = QPixmap.fromImage(image).scaled(self.width*self.size_factor, self.height*self.size_factor)
        return pixmap
        
    def add_grid(self, pixmap):
        painter = QPainter(pixmap)
        pen = QPen(QColor('gray'))
        pen.setWidth(1)
        painter.setPen(pen)
        for x in range(self.width):
            painter.drawLine(x * self.size_factor, 0, x * self.size_factor, self.height * self.size_factor)
        for y in range(self.height):
            painter.drawLine(0, y * self.size_factor, self.width * self.size_factor, y * self.size_factor)
        painter.end()
        
        return pixmap
        
    def show_pixmap(self, pixmap):
        label = QLabel(self)
        label.setPixmap(pixmap)
        self.setCentralWidget(label)
        
def on_memory_modified(memory_path, mem, screen):
    mem.read_memory()
    screen.get_px(mem.data_memory)
    image = screen.update_image()
    pixmap = screen.image_to_pixmap(image)
    screen.add_grid(pixmap)
    screen.show_pixmap(pixmap)
    screen.show()
    
def main():
    app = QApplication()
    
    memory_path = os.path.join(os.getcwd(), "memory/.memory.bin")
    print(memory_path)
    mem = Memory(memory_path)
    screen = Screen(mem)
    on_memory_modified(memory_path, mem, screen)
    watcher = QFileSystemWatcher([memory_path])
    watcher.fileChanged.connect(lambda: on_memory_modified(memory_path, mem, screen))
    
    app.exec()

if __name__ == '__main__':
    main()
