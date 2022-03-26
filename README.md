
# Ising Problem using CUDA parallelisation
## Versions
 * V0: sequential version
 * V1: One thread per moment
 * V2: One block (bxb) of moments per thread
 * V3: One block of moments per cuda block using shared memory

## Results:
 ### V0 times
 <img width="364" alt="Screenshot 2022-03-26 at 7 26 51 PM" src="https://user-images.githubusercontent.com/81708900/160250678-7f3ba165-e401-40ce-987b-23a20da33b6f.png">
 
 ### V1 times
 <img width="374" alt="Screenshot 2022-03-26 at 7 29 32 PM" src="https://user-images.githubusercontent.com/81708900/160250725-daad2290-4e8d-4200-a612-d88d776128a7.png">
 
 ### V2 times
 <img width="358" alt="Screenshot 2022-03-26 at 7 30 10 PM" src="https://user-images.githubusercontent.com/81708900/160250757-9eec9a8d-8e50-43bd-ac40-f752cc7507e7.png">
 
 ### V3 times
 <img width="372" alt="Screenshot 2022-03-26 at 7 31 13 PM" src="https://user-images.githubusercontent.com/81708900/160250787-21e174da-e93a-4278-a3fc-c3896bc7a853.png">
