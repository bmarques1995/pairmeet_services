# Services

The first planning architecture is based on split the problem in individual services.

Each one responsible for one task

## User

Controls the user data, like:

- Birthdate
- Login info
- City
- Search radius
- Pets

## Pet

Controls the pets data:

- Birthdate
- Pedigree
- Owner (the pet, mandatorily, is linked to an owner, at database level)
- Breed (there will be avaliable, a list of breeds to register the pet, cannot be empty)
- Validated data

## Pedigree Validation

Let the admins validate the pedigree of the pet:

## Pet Interaction

- Like/Dislike
- Match

## Chat

- Message
