/**
 * CSC A48 - Intro to Computer Science II, Summer 2020
 * 
 * Assignment 3 - Graffit
 * 
 * Graphs &
 * Recursion
 * Assignment
 * For
 * Freshmen
 * In
 * Toronto
 *
 * (I am so proud of that initialism.)
 * 
 * This is the program file where you will implement your solution for
 * assignment 3. Please make sure you read through this file carefully
 * and that you understand what is provided and what you need to complete.
 * 
 * You will also need to have read the handout carefully. 
 * 
 * Parts where you have to implement functionality are clearly labeled TODO
 * 
 * Be sure to test your work thoroughly, our testing will be extensive
 * and will check that your solution is *correct*, not only that it
 * provides functionality.
 * 
 * (c) 2020 William Song, Mustafa Quraish
 **/

#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX_STR_LEN 1024
#ifndef __testing
#define MAT_SIZE 3	// A small graph
#endif

typedef struct user_struct {
  char name[MAX_STR_LEN];
  struct friend_node_struct* friends;
  struct brand_node_struct* brands;
  bool visited;
} User;

typedef struct friend_node_struct {
  User* user;
  struct friend_node_struct* next;
} FriendNode;

typedef struct brand_node_struct {
  char brand_name[MAX_STR_LEN];
  struct brand_node_struct* next;
} BrandNode;

/** Note: We are reusing the FriendNode here as a Linked List for allUsers.
  * This is usually bad coding practice but it will allow us to reuse the
  * helper functions.
  **/
FriendNode* allUsers; 

int brand_adjacency_matrix[MAT_SIZE][MAT_SIZE];
char brand_names[MAT_SIZE][MAX_STR_LEN];

/**
 * Checks if a user is inside a FriendNode LL.
 **/
bool in_friend_list(FriendNode *head, User *node) {
  for (FriendNode *cur = head; cur != NULL; cur = cur->next) {
    if (strcmp(cur->user->name, node->name) == 0) {
      return true;
    }
  }
  return false;
}

/**
 * Checks if a brand is inside a BrandNode LL.
 **/
bool in_brand_list(BrandNode *head, char *name) {
  for (BrandNode *cur = head; cur != NULL; cur = cur->next) {
    if (strcmp(cur->brand_name, name) == 0) {
      return true;
    }
  }
  return false;
}

/**
 * Inserts a User into a FriendNode LL in sorted position. If the user 
 * already exists, nothing is done. Returns the new head of the LL.
 **/
FriendNode *insert_into_friend_list(FriendNode *head, User *node) {
  if (node == NULL) return head;

  if (in_friend_list(head, node)) {
    printf("User already in list\n");
    return head;
  }
  FriendNode *fn = calloc(1, sizeof(FriendNode));
  fn->user = node;
  fn->next = NULL;

  if (head == NULL)
    return fn;
    
  if (strcmp(head->user->name, node->name) > 0) {
    fn->next = head;
    return fn;
  } 

  FriendNode *cur;
  for (cur = head; cur->next != NULL && strcmp(cur->next->user->name, node->name) < 0;
       cur = cur->next)
    ;
  fn->next = cur->next;
  cur->next = fn;
  return head;
}

/**
 * Inserts a brand into a BrandNode LL in sorted position. If the brand 
 * already exists, nothing is done. Returns the new head of the LL.
 **/
BrandNode *insert_into_brand_list(BrandNode *head, char *node) {
  if (node == NULL) return head;

  if (in_brand_list(head, node)) {
    printf("Brand already in list\n");
    return head;
  }
  BrandNode *fn = calloc(1, sizeof(BrandNode));
  strcpy(fn->brand_name, node);
  fn->next = NULL;

  if (head == NULL)
    return fn;
    
  if (strcmp(head->brand_name, node) > 0) {
    fn->next = head;
    return fn;
  } 

  BrandNode *cur;
  for (cur = head; cur->next != NULL && strcmp(cur->next->brand_name, node) < 0;
       cur = cur->next)
    ;
  fn->next = cur->next;
  cur->next = fn;
  return head;
}

/**
 * Deletes a User from FriendNode LL. If the user doesn't exist, nothing is 
 * done. Returns the new head of the LL.
 **/
FriendNode *delete_from_friend_list(FriendNode *head, User *node) {
  if (node == NULL) return head;

  if (!in_friend_list(head, node)) {
    printf("User not in list\n");
    return head;
  }

  if (strcmp(head->user->name, node->name) == 0) {
    FriendNode *temp = head->next;
    free(head);
    return temp;
  }

  FriendNode *cur;
  for (cur = head; cur->next->user != node; cur = cur->next)
    ;

  FriendNode *temp = cur->next;
  cur->next = temp->next;
  free(temp);
  return head;
}

/**
 * Deletes a brand from BrandNode LL. If the user doesn't exist, nothing is 
 * done. Returns the new head of the LL.
 **/
BrandNode *delete_from_brand_list(BrandNode *head, char *node) {
  if (node == NULL) return head;

  if (!in_brand_list(head, node)) {
    printf("Brand not in list\n");
    return head;
  }

  if (strcmp(head->brand_name, node) == 0) {
    BrandNode *temp = head->next;
    free(head);
    return temp;
  }

  BrandNode *cur;
  for (cur = head; strcmp(cur->next->brand_name, node) != 0; cur = cur->next)
    ;

  BrandNode *temp = cur->next;
  cur->next = temp->next;
  free(temp);
  return head;
}

/**
 * Prints out the user data.
 **/
void print_user_data(User *user) {
  printf("User name: %s\n", user->name);
  printf("Friends:\n");
  for (FriendNode *f = user->friends; f != NULL; f = f->next) {
    printf("   %s\n", f->user->name);
  }
  printf("Brands:\n");
  for (BrandNode *b = user->brands; b != NULL; b = b->next) {
    printf("   %s\n", b->brand_name);
  }
}

/**
 * Get the index into brand_names for the given brand name. If it doesn't
 * exist in the array, return -1
 **/
int get_brand_index(char *name) {
  for (int i = 0; i < MAT_SIZE; i++) {
    if (strcmp(brand_names[i], name) == 0) {
      return i;
    }
  }
  printf("brand '%s' not found\n", name);
  return -1; // Not found
}

/**
 * Print out brand name, index and similar brands.
 **/
void print_brand_data(char *brand_name) {
  int idx = get_brand_index(brand_name);
  if (idx < 0) {
    printf("Brand '%s' not in the list.\n", brand_name);
    return;
  }
  printf("Brand name: %s\n", brand_name);
  printf("Brand idx: %d\n", idx);
  printf("Similar brands:\n");
  for (int i = 0; i < MAT_SIZE; i++) {
    if (brand_adjacency_matrix[idx][i] == 1 && strcmp(brand_names[i], "") != 0) {
      printf("   %s\n", brand_names[i]);
    }
  }
}

/**
 * Read from a given file and populate a the brand list and brand matrix.
 **/
void populate_brand_matrix(char* file_name) {
    // Read the file
    char buff[MAX_STR_LEN];
    FILE* f = fopen(file_name, "r");
    fscanf(f, "%s", buff);
    char* line = buff;
    // Load up the brand_names matrix
    for (int i = 0; i < MAT_SIZE; i++) {
        if (i == MAT_SIZE - 1) {
            strcpy(brand_names[i], line);
            break;
        }
        int index = strchr(line, ',') - line;
        strncpy(brand_names[i], line, index);
        line = strchr(line, ',') + sizeof(char);
    }
    // Load up the brand_adjacency_matrix
    for (int x = 0; x < MAT_SIZE; x++) {
        fscanf(f, "%s", buff);
        for (int y = 0; y < MAT_SIZE; y++) {
            int value = (int) buff[y*2];
            if (value == 48) { value = 0; }
            else {value = 1;}
            brand_adjacency_matrix[x][y] = value;
        }
    }
}


// Users
/**TODO: Complete this function
 * Creates and returns a user. Returns NULL on failure.
 **/
User* create_user(char* name){
  //Create space in memory for a new User node.
  User* new_user = (User*)calloc(1, sizeof(User));
  //Initialize values for the new User node.
  strcpy(new_user->name, name);
  new_user->friends = NULL;
  new_user->brands = NULL;
  new_user->visited = NULL;
  //Case where a user with name already exists on Graffit.
  if(in_friend_list(allUsers, new_user)){
    free(new_user);
    return NULL;
  }
  //Case where name is an empty string, return NULL.
  if(strcmp(name, "") == 0){
    free(new_user);
    return NULL;
  }
  //Add the new user to allUsers.
  allUsers = insert_into_friend_list(allUsers, new_user);
  return new_user; 
}

/**TODO: Complete this function
 * Deletes a given user. 
 * Returns 0 on success, -1 on failure.
 **/
int delete_user(User* user){
  //Case where a user is not on Graffit.
  if(!in_friend_list(allUsers, user)){
    return -1;
  }
  //Case where the username is blank or the user is NULL.
  if(strcmp(user->name, "") == 0 || user == NULL){
    return -1;
  }
  //Delete the user from other users friend LL.
  FriendNode* p = allUsers;
  while(p != NULL){
    if(in_friend_list(p->user->friends, user)){
      p->user->friends = delete_from_friend_list(p->user->friends, user);
      user->friends = delete_from_friend_list(user->friends, p->user);
    }
    p = p->next;
  }
  //Delete the user from allUsers.
  allUsers = delete_from_friend_list(allUsers, user);
  free(user);
  return 0;
}

/**TODO: Complete this function
 * Create a friendship between user and friend.
 * Returns 0 on success, -1 on failure.
 **/
int add_friend(User* user, User* friend){
  //Case where one or the other user is NULL.
  if(user == NULL || friend == NULL){
    return -1;
  }
  //If a blank username has been given.
  if(strcmp(user->name, "") == 0 || strcmp(friend->name, "") == 0){
    return -1;
  }
  //First we must check if these are real users of Graffit.
  if(!in_friend_list(allUsers, user) || !in_friend_list(allUsers, friend)){
    return -1;
  }
  //If there already is a friendship between both the user and friend then we return -1.
  if(in_friend_list(user->friends, friend) && in_friend_list(friend->friends, user)){
    return -1;
  }
  //Case where user and friend are the same users.
  if(strcmp(user->name, friend->name) == 0){
    return -1;
  }
  //Utilize the helper function insert_into_friend_list() to insert friend into user's friend list.
  user->friends = insert_into_friend_list(user->friends, friend);
  //Utilize the helper function insert_into_friend_list() to insert user into friend's friend list.   
  friend->friends = insert_into_friend_list(friend->friends, user);
  return 0;
}

/**TODO: Complete this function
 * Removes a friendship between user and friend.
 * Returns 0 on success, -1 on faliure.
 **/
int remove_friend(User* user, User* friend){
  //Case where one or the other user is NULL.
  if(user == NULL || friend == NULL){
    return -1;
  }
  //If a blank username has been given.
  if(strcmp(user->name, "") == 0 || strcmp(friend->name, "") == 0){
    return -1;
  }
  //First we must check if these are real users of Graffit.
  if(!in_friend_list(allUsers, user) || !in_friend_list(allUsers, friend)){
    return -1;
  }
  //If there is no friendship between both user and friend then we return -1.
  if(!in_friend_list(user->friends, friend) && !in_friend_list(friend->friends, user)){
    return -1;
  }
  //Case where user and friend are the same users.
  if(strcmp(user->name, friend->name) == 0){
    return -1;
  }
  //Utilize the helper function delete_from_friend_list() to get rid of the friendship on user's end.
  user->friends = delete_from_friend_list(user->friends, friend);
  //Utilize the helper function delete_from_friend_list() to get rid of the friendship on friend's end.
  friend->friends = delete_from_friend_list(friend->friends, user);
  return 0;
}

/**TODO: Complete this function
 * Creates a follow relationship, the user follows the brand.
 * Returns 0 on success, -1 on faliure.
 **/
int follow_brand(User* user, char* brand_name){
  //Case where user is NULL.
  if(user == NULL){
    return -1;
  }
  //If a blank username has been given.
  if(strcmp(user->name, "") == 0){
    return -1;
  }
  //First we must check if user is a real user of Graffit.
  if(!in_friend_list(allUsers, user)){
    return -1;
  }
  //Just in case but highly unlikely case where brand_name is the same as the user's name.
  if(strcmp(user->name, brand_name) == 0){
    return -1;
  }
  //We must also check that the brand_name is a real brand on Graffit.
  int brand_index = get_brand_index(brand_name);
  if(brand_index == -1){
    return -1;
  }
  //If the user already follows brand_name then return -1.
  if(in_brand_list(user->brands, brand_name)){
    return -1;
  }
  //Utilize the helper function insert_into_brand_list() to insert the brand_name into user's followed brands.
  user->brands = insert_into_brand_list(user->brands, brand_name);
  return 0;
}

/**TODO: Complete this function
 * Removes a follow relationship, the user unfollows the brand.
 * Returns 0 on success, -1 on faliure.
 **/
int unfollow_brand(User* user, char* brand_name){
  //Case where user is NULL.
  if(user == NULL){
    return -1;
  }
  //If a blank username has been given.
  if(strcmp(user->name, "") == 0){
    return -1;
  }
  //First we must check if user is a real user of Graffit.
  if(!in_friend_list(allUsers, user)){
    return -1;
  }
  //Just in case but highly unlikely case where user's name is the same as brand_name.
  if(strcmp(user->name, brand_name) == 0){
    return -1;
  }
  //We must also check that the brand_name is a real brand on Graffit.
  int brand_index = get_brand_index(brand_name);
  if(brand_index == -1){
    return -1;
  }
  //If the user already doesn't follow the brand then return -1.
  if(!in_brand_list(user->brands, brand_name)){
    return -1;
  }
  //Utilize the helper function delete_from_brand_list() to delete brand_name from the brands that user follows.
  user->brands = delete_from_brand_list(user->brands, brand_name);
  return 0;
}

/**TODO: Complete this function
 * Return the number of mutual friends between two users.
 **/
int get_mutual_friends(User* a, User* b){
  //Create a accumulator variable to keep track of how many mutual friends exist between a and b.
  int counter = 0;
  //Create a temp to store the head for the friends LL's of a.
  FriendNode* p = a->friends;
  //Loop through both the LL's to see if any mutuals exist.
  while(p != NULL){
    //If there is a mutual between a and b add one to counter.
    if(in_friend_list(b->friends, p->user)){
      counter++;
    }
    p = p->next;
  }
  return counter;
}

/**TODO: Complete this function
 * A degree of connection is the number of steps it takes to get from
 * one user to another.
 * 
 * For example, if X & Y are friends, then we expect to recieve 1 when calling
 * this on (X,Y). Continuing on, if Y & Z are friends, then we expect to
 * recieve 2 when calling this on (X,Z).
 * 
 * Returns a non-negative integer representing the degrees of connection
 * between two users, -1 on failure.
 **/
int get_degrees_of_connection(User* a, User* b){
 //To find the shortest path from a to b we need to utilize DFS from the Unit 5 Reading.
  //Base Case.
  if(strcmp(a->name, b->name) == 0){
    return 0;
  }
  //Recursion Case.
  else{
    //We will go through the friends of a.   
    FriendNode* p = a->friends;    
    while(p != NULL){
      //If we don't set visited to true and don't check if we've visited an infinite loop will occur.
      if(p->user->visited != true){
        p->user->visited = true;
        int num = 0;
        num = get_degrees_of_connection(p->user, b);    
        if(num != -1){
          num++;
          return num;
        }
        //if we are back to the first call dont return instead store this num somewhere then find other paths and return the smallest num
      }  
      p = p->next;
    }
    return -1;
  } 
}


// Brands
/**TODO: Complete this function
 * Marks two brands as similar.
 **/
void connect_similar_brands(char* brandNameA, char* brandNameB){
  //Find the indicies of brandNameA and brandNameB utilizing the helper function get_brand_index().
  int a_index = get_brand_index(brandNameA);
  int b_index = get_brand_index(brandNameB);
  //Change the relationship between brandNameA and brandNameB to connect a.k.a 1.
  if(a_index != -1 && b_index != -1){
    brand_adjacency_matrix[a_index][b_index] = 1;
    brand_adjacency_matrix[b_index][a_index] = 1;
  } 
}

/**TODO: Complete this function
 * Marks two brands as not similar.
 **/
void remove_similar_brands(char* brandNameA, char* brandNameB){
  //Find the indicies of brandNameA and brandNameB utilizing the helper function get_brand_index().
  int a_index = get_brand_index(brandNameA);
  int b_index = get_brand_index(brandNameB);
  //Change the relationship between brandNameA and brandNameB to unconnected a.k.a 0.
  if(a_index != -1 && b_index != -1){
    brand_adjacency_matrix[a_index][b_index] = 0;
    brand_adjacency_matrix[b_index][a_index] = 0;
  } 
}


// Harder ones
/**TODO: Complete this function
 * Returns a suggested friend for the given user, returns NULL on failure.
 * See the handout for how we define a suggested friend.
 **/
User* get_suggested_friend(User* user){
  //Case where the user is NULL.
  if(user == NULL){
    return NULL;
  }
  //If a blank username has been given.
  if(strcmp(user->name, "") == 0){
    return NULL;
  }
  //Check if user is on Graffit.
  if(!in_friend_list(allUsers, user)){
    return NULL;
  }
  //Create a temp that stores allUsers.
  FriendNode* p = allUsers;
  //Create a User node that will store the friend we want to suggest for user.
  User* suggested_friend = NULL;
  //Create a variable that will be used for comparison to see which user has the most brands in common with user.
  int common_brands = 0;
  //Loop through allUsers.
  while(p != NULL){
    //Make sure that we can only get suggested friends if they aren't the user themselves or aren't in user's friends LL already.
    if(!in_friend_list(user->friends, p->user) && strcmp(p->user->name, user->name) != 0){
      //Create a temp that stores the potential friend's brand LL.
      BrandNode* q = p->user->brands;
      //Create an accumulator that will be used to count how many brands are in common.
      int counter = 0;
      //Loop through p's brand LL.
      while(q != NULL){
        if(in_brand_list(user->brands, q->brand_name)){
          counter++;
        }
        q = q->next;
      }
      //To not have a problem with the other if statements we must have a suggested_friend to start.
      if(suggested_friend == NULL){
        suggested_friend = p->user;
      }
      //Case where the number of brands that are common between p and user are greater than any previous user and user.
      if(counter > common_brands){
        common_brands = counter;
        suggested_friend = p->user;
      }
      //Case where the number pf brands that are common between p and user are equal to any previous user and user.
      else if(counter == common_brands){
        if(strcmp(p->user->name, suggested_friend->name) > 0){
          suggested_friend = p->user;
        } 
      }
    }
    p = p->next;
  }
  return suggested_friend;
}

/**TODO: Complete this function
 * Friends n suggested friends for the given user.
 * See the handout for how we define a suggested friend.
 * Returns how many friends were successfully followed.
 **/
int add_suggested_friends(User* user, int n){
  //Case where user is NULL.
  if(user == NULL){
    return 0;
  }
  //If a blank username has been given.
  if(strcmp(user->name, "") == 0){
    return 0;
  }
  //Check if user is on Graffit.
  if(!in_friend_list(allUsers, user) || n <= 0){
    return 0;
  }
  //Create a User node to store the suggested_friend received from get_suggested_friend().
  User* suggested_friend = NULL;
  //Create an accumulator variable that will count the number of succesful friends added.
  int friends = 0;
  //Create an accumulator variable that will make sure we don't surpass n.
  int counter = 0;
  //Keep passing user into get_suggested_friend until n friends have been added.
  while(counter != n){
    suggested_friend = get_suggested_friend(user);
    //In the case that every possible user has been befriended by user, we want to make sure we don't befriend NULL.
    if(suggested_friend != NULL){
      add_friend(user, suggested_friend);
      friends++;  
    } 
    counter++;
  }
  return friends;  
}

/**TODO: Complete this function
 * Follows n suggested brands for the given user.
 * See the handout for how we define a suggested brand.     
 * Returns how many brands were successfully followed. 	  	
 **/
int follow_suggested_brands(User* user, int n){
  //Case where user is NULL.
  if(user == NULL){
    return 0;
  }
  //If a blank username has been given.
  if(strcmp(user->name, "") == 0){
    return 0;
  }
  //Check if user is on Graffit.
  if(!in_friend_list(allUsers, user) || n <= 0){
    return 0;
  }
  //Create a string that will store the suggested brand.
  char suggested_brand[MAX_STR_LEN];
  //Create an accumulator variable that keeps count of how many brands are followed by user.
  int brands = 0;
  //Create an accumulator variable that makes sure we never surpass n.
  int count = 0;
  //Loop through and find as many suggested brands and follow them until n is reached.
  while(count != n){
    //Create an accumulator that will be used to compare if a brand is the most similar to brands in user's followed brands.
    int similar_brands = 0;
    //Make sure to start with suggested_brand as empty, because we have to loop through the following loop many times.
    strcpy(suggested_brand, "");
    //Loop through all the brands in brand_names.
    for(int i = 0; i < MAT_SIZE; i++){    
      //Create an accumulator that keeps track of the number of similar brands to the ones in user's brand LL.
      int counter = 0;
      //Create a temp that stores user's brand LL.
      BrandNode* p = user->brands;
      //Make sure that we only look at brands that aren't already followed by user.
      if(!in_brand_list(p, brand_names[i])){
        //Loop through user's brand LL and see if there is a connection between the current brand.
        while(p != NULL){
          if(brand_adjacency_matrix[i][get_brand_index(p->brand_name)] == 1){
            counter++;
          }
          p = p->next;
        }
        //This case is just to start with something.
        if(strcmp(suggested_brand, "") == 0){
          strcpy(suggested_brand, brand_names[i]);
        }
        //This is the case where the current brand is more similar to user's followed brands than any other previous brand.
        if(counter > similar_brands){
          similar_brands = counter;
          strcpy(suggested_brand, "");
          strcpy(suggested_brand, brand_names[i]);
        }
        //This is the case where the current brand is equally similar to user's followed brands as a previous brand.
        else if(counter == similar_brands){
          if(strcmp(brand_names[i], suggested_brand) > 0){
            strcpy(suggested_brand, "");
            strcpy(suggested_brand, brand_names[i]);
          }
        }
      }
    }
    //In the case that there are no more brands left to be followed do not follow an empty string.
    if(strcmp(suggested_brand, "") != 0){
      follow_brand(user, suggested_brand);
      brands++;
    }
    count++;
  }
  return brands;
}
