# That Project is done by

>> 1. Sineth Dhananjay
>> 2. Isuru Liyanage


* Project has a low level architecture and a high level Architecture

# Low Level Architecture

* That is consist of 13 files
* For the execute example, that need a file 'order.csv'

- Example

```Text
Client Order ID,Instrument,Side,Quantity,Price
aa13,Rose,2,100,55
aa14,Rose,1,100,12p
aa15,Rose,2,100,45
aa16,Rose,1,200,4
aa17,Royse,2,100,15
aa18,Rose,2,100,65
```

* For the Run software, First you need to compile the project

- For That , use that command

```bash
g++ -g main.cpp order.cpp RedBlackTree.cpp ValidateOrder.cpp BuyHandler.cpp Cache.cpp -o out
```

* After that you can run the command

- For That , use that command

```bash
./out
```

- Then you can see the output in 'execution_rep.csv'


# High Level Architecture

That consist three parts

1. Authenticator
2. Backend
3. GUI

### Authenticator

That had three three functions

1. Sign Up
2. Log In
3. Token Validating

* without login you can't do order process
* First you need to make a account in our system
* After that we are create a account
 
> In the inner process we are add data to our database and generate a token. 
> For every login we are creating new token. 
> As well as that has auto login facility. 

- If you need to run this use that commands

```bash
cd Authenticater
cd build
cmake ..
make
./Authenticater
```

### Backend

That had three functions

1. New Order
2. Get History

* without login, you can't do order process

> In the inner process we are add data to our database and generate a token.
> For every login we are creating new token.
> As well as that has auto login facility.

- If you need to run this use that commands

```bash
cd Backend
cd build
cmake ..
make
./Backend
```

> More Information about our API , Use our postman link.
> 
> [Postman Link](https://www.postman.com/martian-eclipse-984811/workspace/lseg-project/collection/26088144-bfb9b26e-9364-4ed5-9cf7-5532f8bfa04a?action=share&creator=26088144)


