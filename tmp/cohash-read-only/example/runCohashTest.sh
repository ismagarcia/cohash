# convert flower_1024.tga -filter point -resize 128x128 flower_128.tga

./example_01
#./example_01 4 20 99 61332125 1 -coh_hash
#./example_01 4 20 99 61332125 1 -rand_hash
./example_01 ./flower_1024.tga 1 99 77016577 1 -coh_hash
./example_01 ./flower_1024.tga 1 99 77016577 1 -rand_hash
#./example_01 ./flower_1024.tga 1 70 77016577 1 -coh_hash
#./example_01 ./flower_1024.tga 1 70 77016577 1 -rand_hash
#./example_01 3200000 20 99 61332125 1 -coh_hash
#./example_01 3200000 20 99 61332125 1 -rand_hash
#./example_01 3200000 20 50 61332125 0 -coh_hash 
#./example_01 3200000 20 50 61332125 0 -rand_hash
#./example_01 3200000 20 99 61332125 0 -coh_hash
#./example_01 3200000 20 99 61332125 0 -rand_hash
