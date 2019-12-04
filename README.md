# infinite-goldfish

Code for Infinite Goldfish project.

Infinite Goldfish is an installation in which a goldfish "types" an “infinitely” long string of letters by unconsciously swimming in an aquarium. It intends to raise the question of whether meaning does exist by demonstrating an absurd practice of looking for meaning in randomness. 

Through Raspberry Pi and Pi camera, a top view of the fish tank is taken and uploaded to a cloud drive for further process every minute. Using Blob Detection, the position of the fish in the aquarium is found and mapped to a specific English letter. The generated string is then passed to a piece of C code, which would create files marking out and counting the meaningful words that appeared in the text. 
