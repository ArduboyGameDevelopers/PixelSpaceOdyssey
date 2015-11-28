require 'chunky_png'
require_relative 'prog_mem'

module ChunkyPNG

  COLOR_BLACK = 0x000000ff
  COLOR_TRANSPARENT = 0xffffff00

  class Canvas
    def sub_image(x, y, w, h)
      raise "Region (#{x},#{y},#{w},#{h}) out of range (#{width}x#{height})" if w < 0 || h < 0 ||
          x < 0 || y < 0 || x + w > width || y + h > height

      sub_pixels = []
      (y..y+h-1).each do |y|
        (x..x+w-1).each do |x|
          pixel = pixels[y * width + x]
          alpha = Color.a pixel
          sub_pixels.push (alpha > 0 && !is_white(pixel) ? COLOR_BLACK : COLOR_TRANSPARENT)
        end
      end
      Image.new w, h, sub_pixels
    end

    def to_prog_mem
      ProgMem.from_png self
    end

    def is_transparent
      pixels.each { |pixel|
        return false if Color.a(pixel) > 0
      }
      true
    end

    private
    def is_white(color)
      (color >> 8) == 0xffffff
    end

  end
end