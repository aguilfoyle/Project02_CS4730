//---INCLUDE(S)---
#include <stdio.h>
#include <stdlib.h>
#include "vm.h"
#include "math.h"
#include "disk.h"
#include "pagetable.h"



//---GLOBAL VARIABLE(S)---
bool keepGoing = true;
int indexForFrame = -1;
int lowestFrequency = 1;
int highestFrequency = 1;
float tempPulledOldFrame;
float tempPulledNewFrame;
//---INSTANTIATION(S)---
void deleteLeastUsed();
void getLowestFrequency();
float frequencyIncrementer(float value);



//Struct needed for linked list data structure
struct Node
{
    float recentlyUsed;
    struct Node * linkedList;
};

struct Node * head = NULL;
struct Node * tail = NULL;


/*
 * NAME: findLowPriorityLeastUsedItem | PARAM: int | RETURN: void
 * PURPOSE: When called this method will get the least used item in the list
 *  and determine by a series of if/else statements if that is the correct
 *  item needed to continue; This method was created by Alan Guilfoyle
 */
void findLowPriorityLeastUsedItem( int getFrame )
{
    //---VARIABLE(S)---
    struct Node * temp = head;
    
    //Will loop through the entire list of items until the end(null)
    while( temp != NULL )
    {
        if( lfuFlag == 2 ) //IF: Enters if PT had a hit
        {
            float frameTemp = temp -> recentlyUsed;
            int freqTemp = ( int )temp -> recentlyUsed;

            frameTemp = temp -> recentlyUsed - freqTemp;

            float tempfreqTemp = frameTemp * 10;

            if( getFrame == roundf(tempfreqTemp) )
            {
                tempPulledOldFrame = temp -> recentlyUsed;

                return; //Exit this method
            }
        }
        else if( lfuFlag == 1 ) //ELSE IF: Enters if PT is FUll
        {
            //Gets the frequency from the item in list
            int roundedTemp = ( int )temp -> recentlyUsed;
            
            //IF HIGHESTFREQ DOESN'T EQUAL LEAST RECENTLY USED FREQ
            if( highestFrequency != roundedTemp )
            {
                //printf("OMGOMGOMG %d %d\n", roundedTemp, lowestFrequency );
                if( roundedTemp > lowestFrequency )
                {
                    //Keep the loop going in lookAtLeastFrequentlyUsed()
                    keepGoing = true;
                    
                    return; //Exit this method
                }
                else
                {
                    //Stop the loop in lookAtLeastFrequentlyUsed()
                    keepGoing = false;
                    
                    return; //Exit this method
                }
            }
            else
            {
                //Ensures the lowest frequency is stored
                getLowestFrequency();
                
                //Keep the loop going in lookAtLeastFrequentlyUsed()
                keepGoing = true;
                //SETS LEAST RECENTLY USED TO tempPulledOldFrame
                tempPulledOldFrame = temp -> recentlyUsed;
                
                return; //Exit this method
            }
        }
        
        temp = temp -> linkedList;
    }
}



/*
 * NAME: getLowestFrequency() | PARAM: None | RETURN: void
 * PURPOSE: When called this will set the lowestFrequency amoung the
 *  items in the list; This method was created by Alan Guilfoyle
 */
void getLowestFrequency()
{
    //---VARIABLE(S)---
    bool boolFlag = true;
    struct Node * temp = head;
    
    int roundedTemp = ( int )temp -> recentlyUsed;
    
    if( roundedTemp != 1 ) //IF: Enters if roundTemp isn't a 1
    {
        lowestFrequency = roundedTemp;
    }
    
    //Will loop through the entire list of items until the end(null)
    while( temp != NULL )
    {
        int roundedTemp = ( int )temp -> recentlyUsed;
        
        if( roundedTemp == 1 )
        {
            lowestFrequency = 1;
            boolFlag = false;
        }
        if( roundedTemp < lowestFrequency && boolFlag != false )
        {
            lowestFrequency = roundedTemp;
        }

        temp = temp -> linkedList;
    }
    
    if( boolFlag == false ) //IF: Enters if there was a 1 present
    {
        //Overkill but that's okay
        lowestFrequency = 1;
    }
}



/*
 * NAME: deleteAnyNode() | PARAM: float | RETURN: int
 * PURPOSE: When called this method will delete any item within the list;
 *  it takes in the item that should be deleted; This method was created 
 *  by Alan Guilfoyle
 */
int deleteAnyNode( float usedRecently )
{
    //---VARIABLE(S)---
    int i = 0;
    int placement = -1;
    
    struct Node * currentlyUsed =
    ( struct Node * )malloc( sizeof( struct Node ));
    struct Node * temp =
    ( struct Node * )malloc( sizeof( struct Node ));
    
    currentlyUsed = head;
    temp = NULL;
    
    if( head == tail )
    {
        head = tail = NULL;
    }
    
    //Nothing in list
    if(( tail ) -> recentlyUsed == usedRecently )
    {
        return -1; //return error
    }
    
    //The first item to be inserted (least recently added)
    if(( head ) -> recentlyUsed == usedRecently )
    {
        //Call deleteLeastUsed() since it deals with least recently added
        deleteLeastUsed();
        
        return 0; //return any number
    }
    
    while( currentlyUsed -> linkedList -> recentlyUsed != usedRecently )
    {
        if( currentlyUsed -> linkedList == NULL )
        {
            return -1; //return error
        }
        currentlyUsed = currentlyUsed -> linkedList;
    }
    
    temp = currentlyUsed -> linkedList;
    placement = temp -> recentlyUsed;
    currentlyUsed -> linkedList = temp -> linkedList;
    
    free( temp );
    
    return placement;
}



/*
 * NAME: deleteLeastUsedNode() | PARAM: None | RETURN: void
 * PURPOSE: When called this method will delete the item at the tail of 
 *  the list (AKA: Least recently used); This method was created by Alan 
 *  Guilfoyle
 */
void deleteLeastUsed()
{
    struct Node * temp = head;
    
    if( head == NULL )
    {
        printf( "Queue is Empty\n" );
    }
    else
    {
        if( head == tail )
        {
            head = tail = NULL;
        }
        else
        {
            frameReference = head -> recentlyUsed;
            head = head -> linkedList;
        }
        
        free( temp );
    }
}



/*
 * NAME: insertNode() | PARAM: float | RETURN: void
 * PURPOSE: When called this method will insert a new item to the
 *  front of the list; This method was created by Alan Guilfoyle
 */
void insertNode( float x )
{
    struct Node * temp =
    ( struct Node * )malloc( sizeof( struct Node ));
    
    temp -> recentlyUsed = x;
    temp -> linkedList = NULL;
    
    if(head == NULL && tail == NULL)
    {
        head = tail = temp;
    }
    else
    {
        tail -> linkedList = temp;
        tail = temp;
    }
}



/*
 * NAME: frequencyIncrementer | PARAM: float | RETURN: float
 * PURPOSE: When this method is called it will increment the frequency
 *  of the specific item in the list that needs to be incremented; This 
 *  method was created by Alan Guilfoyle
 */
float frequencyIncrementer( float value )
{
    //Reference for reader: FREQ-FRAME
    //                        1 . 2
    float incremented = 0;
    
    if( lfuFlag == 2 ) //IF: Enters if there was a hit
    {
        //Gets item from list
        findLowPriorityLeastUsedItem( ( int )value );
        //Increments frequency
        incremented = tempPulledOldFrame + 1;
        //Sets the item to tempPulledNewFrame to be inserted later
        tempPulledNewFrame = incremented;
        
        //Increase Highest Frecrency global variable if needed
        if( highestFrequency < ( int )incremented )
        {
            highestFrequency++;
        }
        
        return incremented; //Return the newly updated item
    }
    else //ELSE: Enters if there was a miss & PT not full
    {
        if( value == 0.0 ) //IF: the frame is 0, special treatment
        {
            //do nothing
            incremented = value + 1.0;
            
            //Increase Highest Frecrency global variable if needed
            if( highestFrequency < roundf( incremented ))
            {
                highestFrequency++;
            }
            
            return incremented; //Return the newly updated item
        }
        else //ELSE: frame is not 0
        {
            //Convert frame to float
            incremented = value / 10;
            //Increase frecrency
            incremented = incremented + 1;
            
            //Increase Highest Frecrency global variable if needed
            if( highestFrequency < roundf( incremented ))
            {
                highestFrequency++;
            }
            
            return incremented; //Return the newly updated item
        }
    }
}



/*
 * NAME: lookAtLeastFrequentlyUsed() | PARAM: None | RETURN: int
 * PURPOSE: When called this method will look at least frequently used
 *  and determine if that is the item that should be replaced. This method 
 *  was created by Alan Guilfoyle
 */
int lookAtLeastFrequentlyUsed()
{
    //---VARIABLE(S)---
    float getLeastUsed = 0;
    int counting = 0;
    
    //Will continue to loop until findLowPriorityLeastUsedItem() sets keepGoing to false
    while( keepGoing )
    {
        //IF: Enters if there hasn't been a hit yet
        if( highestFrequency == 1 )
        {
            //Sets the least used item in list
            tempPulledOldFrame = head -> recentlyUsed;
            //So we loop can exit & hit the right nxt if statement
            keepGoing = false;
        }
        else //ELSE: Enters if there has been @ least 1 hit
        {
            //Looks at current priority == highest priority currently
            findLowPriorityLeastUsedItem( 0 );
        }
        
        
        if( keepGoing == true ) //IF: Enters when highest priority is least used item
        {
            //Sets the least used item in list
            tempPulledOldFrame = head -> recentlyUsed;
            //Deletes least used item in list
            deleteLeastUsed();
            //Inserts the least used item in list again but at the tail
            insertNode( tempPulledOldFrame );
        }
        else //ELSE: Enters when least used has also lowest priority
        {
            //Setting tempPulledOldFrame to the least frequently used item
            tempPulledOldFrame = head -> recentlyUsed;
            
            //OSOLATES JSUT THE FREQ TO USE TO SUBTRACT IT LATER
            int intVersionLeastUsed = ( int )tempPulledOldFrame;
            
            //Gets the .0 part of the float
            float frameTemp = tempPulledOldFrame - intVersionLeastUsed;
            //Gets the frame by times 10 by 0.? spot
            int returnThisFrame = 10 *  frameTemp;
            
            //Delete it from list
            deleteLeastUsed();
            //Add it to the tail of list
            insertNode( 1 + frameTemp );
            
            return returnThisFrame;
        }
    }
    
    return 0; //Return doesn't matter; Shouldn't reach this
}



/*
 * NAME: printPageTableStats() | PARAM: None | RETURN: void
 * PURPOSE: When called this method will generate a random number
 *  inbetween 0 and MAX_FRAME. This method was created by professor 
 *  Lee but filled in by Alan Guilfoyle
 */
int randomPageReplacement()
{
    //Gets new random number
    int next = rand() % MAX_FRAME;
       
    return next; //Return next victim frame
}



/*
 * NAME: printPageTableStats | PARAM: None | RETURN: void
 * PURPOSE: This
 *  method was created by professor Lee but filled in by Alan Guilfoyle
 */
int fifoPageReplacement()
{
    //Increments indexForFrame
    indexForFrame = indexForFrame + 1;
    
    return indexForFrame % MAX_FRAME; //Returns new victim frame
}



/*
 * NAME: printPageTableStats | PARAM: None | RETURN: void
 * PURPOSE: When called this method will enter a series of if/else
 *  statements depending on if: There a MISS & PT isn't full, a MISS 
 *  & PT is full, or  *  HIT; Flags are set in pagetable.c; This 
 *  method was created by professor Lee but filled in by Alan Guilfoyle
 */
int lruPageReplacement()
{
    if( lruFlag == 0 ) //IF: Enters if there is a Miss & Not Full
    {
        insertNode( frameReference );
        
        return -1; //Return any number
    }
    else if( lruFlag == 1 ) //ELSE IF: Enters if there is a Miss & Full
    {
        //Delete least used from list
        deleteLeastUsed();
        //Insert new frame into list
        insertNode( frameReference );
        
        return frameReference; //Return the item inserted in list
    }
    else //ELSE: Enters if there is a Hit
    {
        //Delete frame from list
        if(!(deleteAnyNode( frameReference ) == -1))
        {
            //Add frame to head
            insertNode( frameReference );
        }

        return -1; //Return any number
    }
}



/*
 * NAME: printPageTableStats | PARAM: None | RETURN: void
 * PURPOSE: When called this method will enter a series of if/else state-
 *  ments depending on if: There's a MISS & PT isn't full, a MISS & PT is full, or
 *  HIT; This method was created by professor Lee but filled in by Alan Guilfoyle
 */
int lfuPageReplacement()
{
    //Sets keepGoing flag back to true
    keepGoing = true;
    //Gets the frame set in pagetable.c & turns it to float
    float frameAsFloat = ( float )frameReference;

    if( lfuFlag == 0 ) //IF: Enters if there is a Miss & PT isn't Full
    {
        /* OVERVIEW OF PURPOSE: This will use the same technique that FIFO method
         * uses to get the next frame; The frequency of that frame is added and 
         * inserted into linked list*/
        
        //---VARIABLE(S)---
        int nxtFrame = 0;
        
        indexForFrame = indexForFrame + 1;
        nxtFrame = indexForFrame % MAX_FRAME;
        
        //Increments frequency of frame
        frameAsFloat = frequencyIncrementer( ( float )nxtFrame );
        //Save frame in linked list
        insertNode( frameAsFloat );
        
        return indexForFrame % MAX_FRAME; //return frame to use
    }
    else if( lfuFlag == 1 ) //ELSE IF: Enters if there is a Miss & PT is Full
    {
        /* OVERVIEW OF PURPOSE: Call lookAtLeastFrequentlyUsed(); Set lowestFrequency 
         * to 1; return the victim frame */
        
        //Returns the victim frame
        int returnMe = lookAtLeastFrequentlyUsed();
        //Set lowest frequency to 1
        lowestFrequency = 1;
        
        return returnMe;//Return FRAME
    }
    else //ELSE: Enters if there is a hit
    {
        /* OVERVIEW OF PURPOSE: Call frequencyIncrementer(); Find the frame hit; 
         * Increment the frecrency; return nothing */
        
        //Increment frequency
        float newEntry = frequencyIncrementer( frameAsFloat );

        //IF: Enters if there is only one item in linked list
        if( indexForFrame == 0 )
        {
            //OVERVIEW: delete from Linked List and re-add it to List
            deleteLeastUsed();      //delete the old value
            insertNode( newEntry ); //insert the new value
        }
        else //ELSE: Enters if more than one item in linked list
        {
            //OVERVIEW: delete from Linked List and re-add it to List
            deleteAnyNode( tempPulledOldFrame );//delete the old value
            insertNode( newEntry );             //insert the new value
        }
    
        return 0;//return nothing
    }
}



/*
 * NAME: page_replacement() | PARAM: None | RETURN: void
 * PURPOSE: If physical memory is full, call this to find a victim page;
 *  This method was provided by the professor;
 */
int page_replacement()
{
    int frame;
    
    if( replacementPolicy == RANDOM )
    {
        frame = randomPageReplacement();
    }
    else if( replacementPolicy == FIFO )
    {
        frame = fifoPageReplacement();
    }
    else if( replacementPolicy == LRU )
    {
        frame = lruPageReplacement();
    }
    else if( replacementPolicy == LFU )
    {
        frame = lfuPageReplacement();
    }
    
    return frame;
}