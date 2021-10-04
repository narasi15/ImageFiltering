FLAGS = -Wall -std=gnu99 -g

all: gaussian_blur image_filter copy edge_detection greyscale scale

gaussian_blur: gaussian_blur.o bitmap.o
	gcc ${FLAGS} -o $@ $^ -lm

copy: copy.o bitmap.o
	gcc ${FLAGS} -o $@ $^ -lm

greyscale: greyscale.o bitmap.o
	gcc ${FLAGS} -o $@ $^ -lm

scale: scale.o bitmap.o
	gcc ${FLAGS} -o $@ $^ -lm

edge_detection: edge_detection.o bitmap.o
	gcc ${FLAGS} -o $@ $^ -lm

image_filter: image_filter.o
	gcc ${FLAGS} -o $@ $^ -lm

%.o: %.c bitmap.h
	gcc ${FLAGS} -c $<

clean:
	rm *.o image_filter gaussian_blur copy greyscale scale edge_detection

test:
	mkdir images -p
	./copy < dog.bmp > images/dog_copy.bmp
	./greyscale < dog.bmp > images/dog_grey.bmp
	./scale 2 < dog.bmp > images/dog_scale.bmp
	./gaussian_blur < dog.bmp > images/dog_blur.bmp
	./edge_detection < dog.bmp > images/dog_edge.bmp
	./image_filter dog.bmp images/dog_piped.bmp
	
