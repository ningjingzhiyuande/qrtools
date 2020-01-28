#include <qrtools_decoder.h>

static VALUE new(VALUE klass, VALUE image)
{
  QrDecoderHandle decoder = qr_decoder_open();

  IplImage * src;
  Data_Get_Struct(image, IplImage, src);
  qr_decoder_set_image_buffer(decoder, src);

  VALUE self = Data_Wrap_Struct(klass, NULL, qr_decoder_close, decoder);
  return self;
}

static VALUE native_decode(VALUE self, VALUE size, VALUE delta)
{
  QrDecoderHandle decoder;

  Data_Get_Struct(self, struct QrDecoderHandle, decoder);
  short stat = qr_decoder_decode(decoder, NUM2INT(size), NUM2INT(delta));

  if((stat & QR_IMAGEREADER_DECODED) == QR_IMAGEREADER_DECODED)
    return Qtrue;

  return Qfalse;
}

static VALUE header(VALUE self)
{
  QrCodeHeader * header = calloc(1, sizeof(QrCodeHeader));
  QrDecoderHandle decoder;

  Data_Get_Struct(self, struct QrDecoderHandle, decoder);

  qr_decoder_get_header(decoder, header);

  if(header)
    return Data_Wrap_Struct(cQRToolsHeader, NULL, free, header);

  free(header);
  return Qnil;
}

static VALUE body(VALUE self)
{
  QrDecoderHandle decoder;
  QrCodeHeader * head;

  VALUE _header = rb_funcall(self, rb_intern("header"), 0);
  if(Qnil == _header) return Qnil;

  Data_Get_Struct(self, struct QrDecoderHandle, decoder);
  Data_Get_Struct(_header, QrCodeHeader, head);

  char *buf = calloc(head->byte_size + 1, sizeof(char));
  VALUE b = Qnil;

  if(qr_decoder_get_body(decoder, (unsigned char *)buf, head->byte_size + 1))
    b = rb_str_new(buf, head->byte_size);

  free(buf);
  return b;
}

static VALUE set_image(VALUE self, VALUE image)
{
  QrDecoderHandle decoder;
  Data_Get_Struct(self, struct QrDecoderHandle, decoder);

  IplImage * src;
  Data_Get_Struct(image, IplImage, src);
  qr_decoder_set_image_buffer(decoder, src);

  return self;
}

static VALUE binarized_image(VALUE self)
{
  QrDecoderHandle decoder;
  Data_Get_Struct(self, struct QrDecoderHandle, decoder);

  IplImage * bin = cvCloneImage(qr_decoder_get_binarized_image_buffer(decoder));

  return QRTools_Wrap_Image(bin);
}

static VALUE coderegion_vertexes(VALUE self)
{
  QrDecoderHandle decoder;
  Data_Get_Struct(self, struct QrDecoderHandle, decoder);

  CvPoint *vertexes = qr_decoder_get_coderegion_vertexes(decoder);
  VALUE cPoint = rb_const_get(cQRToolsImage, rb_intern("Point"));

  VALUE list = rb_ary_new();
  int i = 0;
  for(i = 0; i < 4; i++) {
    CvPoint p = vertexes[i];
    VALUE point = rb_funcall(cPoint, rb_intern("new"), 2, INT2NUM(p.x), INT2NUM(p.y));
    rb_ary_push(list, point);
  }

  return list;
}

VALUE cQRToolsDecoder;
void init_qrtools_decoder()
{
  VALUE qrtools  = rb_define_module("QRTools");
  VALUE klass    = rb_define_class_under(qrtools, "Decoder", rb_cObject);

  cQRToolsDecoder = klass;

  rb_define_singleton_method(klass, "new", new, 1);

  rb_define_private_method(klass, "native_decode", native_decode, 2);

  rb_define_method(klass, "header", header, 0);
  rb_define_method(klass, "body", body, 0);
  rb_define_method(klass, "image=", set_image, 1);
  rb_define_method(klass, "binarized_image", binarized_image, 0);
  rb_define_method(klass, "coderegion_vertexes", coderegion_vertexes, 0);
}
