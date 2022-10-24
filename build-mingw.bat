rmdir /Q /S build && mkdir build & cd build && cmake .. -G "Ninja" && cd ..
cd build && ninja
cd ..