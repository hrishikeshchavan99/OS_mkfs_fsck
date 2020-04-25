Extra libraries other than the ones used in class code
1. math.h
2. uuid.h

linkers require

-lm for math.h
-luuid for uuid.h

-b option can be used to give some specific block size

Rest compilation and running is same as normal.


Part completed:

1. MKFS complete and running, mounts in all cases
- Mkfs runs and is tested on a 2GB aand 8 GB partition.
- Block sizes considered- 1,2,4k
- Mounts in all the three cases with -b option
- Attaching screenshots of the following three cases: (To differentiate between the screenshots, the mount options filed has been         deliberately set to 0 in our code)
    - 2GB partition, Block size 1K
    - 2GB partition, Block size 4k
    - 8GB partition, Block size 2k
- On using debugfs for the default mkfs and for the mkfs code wriiten by us all the fields match. 
- Generalised formulae have been used hardly there is anything which is hardcoded
- Revision 1 of the EXT2 has been implemented
- Note: Block sizes greater than 4k are taken as 4k, for ones between 1024 and 2048 thay are taken as 1024, for block sizes between 2048 and 4096 they are taken as 2048. For block sizes less than 1024, an error prompting incorrect block size is prompted.

2. FSCK, not complete
- Superblock checked
- Block group descriptors checked
- Code for constructing the tree is ready. 
- Facing problems with finding DID, DDID inode numbers
- Block/Inode bitmap not done
- no specific command to run the code


Zip folder having the screenshots has been uploaded
