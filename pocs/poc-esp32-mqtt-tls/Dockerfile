FROM espressif/idf:v4.4

#
# instalacion de ESP-IDF-LIB
#
WORKDIR /opt/esp
RUN git clone https://github.com/UncleRus/esp-idf-lib.git
ENV IDF_LIB_PATH=/opt/esp/esp-idf-lib

# VOLUME /project
WORKDIR /project
COPY . .
RUN . /opt/esp/idf/export.sh && idf.py clean build
# RUN apt-get update && apt-get install ruby-full -y
# RUN gem install ceedling

RUN echo $IDF_PATH


