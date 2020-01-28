= QRTools

* http://qrtools.rubyforge.org/

== DESCRIPTION:

QRTools is a library for decoding QR Codes.  It relies on
libdecodeqr for decoding.

== FEATURES/PROBLEMS:

* Running the tests will take a picture of you.
* Currently awesome.

== SYNOPSIS:

  ###
  # Encode a QR code
  require 'qrtools'
  require 'tempfile'

  filename = File.join(Dir::tmpdir, 'test.png')
  File.open(filename, 'wb') { |fh|
    fh.write QRTools::QRCode.encode('http://tenderlovemaking.com/').to_png
  }

  ###
  # Decode A QR code from a file
  img = QRTools::Image.load(filename)
  decoder = QRTools::QRCode.decode(img)
  puts decoder.body

  ###
  # Decode a photo from the webcam
  QRTools::UI::Camera.new(0) do |camera|
    puts QRTools::QRCode.decode(camera.capture).body
  end

== REQUIREMENTS:

* opencv
* qrencode

== INSTALL:

On OS X:

  * port install opencv qrencode
  * gem install qrtools

== LICENSE:

(The MIT License)

Copyright (c) 2009:

* {Aaron Patterson}[http://tenderlovemaking.com]

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
'Software'), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
