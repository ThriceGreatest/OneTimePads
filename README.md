# OneTimePads
5 small programs that work in tandem to encrypt or decrypt information using a one-time pad system. This was coded in C, for the class CS344/OPERATING SYSTEMS

To use, start by compiling all the programs with by executing './compileall'
Then, generate a key of specificied length by executing, "./keygen [LENGTH] > [newfile]"

Next, you must setup both the encryption and decryption servers using the following commands:
 "./enc_server [PORT1] &"
         and
 "./dec_server [PORT2] &" 
 **choose different ports. PORT1 != PORT2**
 
Finally, you can encrypt or decrypt a plaintext file using the following commands:
  "./enc_client [plaintext] [keyfile] [PORT1]"
           or
  "./dec_client [ciphertext] [keyfile] [PORT2]"
 
