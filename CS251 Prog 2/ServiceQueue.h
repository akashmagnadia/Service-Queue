#ifndef _SERVICE_QUEUE_H
#define _SERVICE_QUEUE_H

#include <iostream>
#include <vector>
#include <utility>

using namespace std;


class ServiceQueue {

private:
	/** Your private data members here!
	*   (you should have NO PUBLIC data members!
	*
	* Nested private types also go here.
	* For example, if your design needs some kind of
	*   structure, you would specify it here.
	*/
	struct qNode {
		bool inQueue;
		int buzzNum;
		qNode *next;
		qNode *prev;

		qNode(bool _inQueue = false, int _buzzNum = -1, qNode* _next = NULL, qNode* _prev = NULL) {
			inQueue = _inQueue;
			buzzNum = _buzzNum;
			next = _next;
			prev = _prev;
		}
	};

	//list of buzzers that have been removed from the queue
	struct unQueuedStack {
		int buzzNum;
		unQueuedStack *next;

		unQueuedStack(int _buzzNum = -1, unQueuedStack* _next = NULL) {
			buzzNum = _buzzNum;
			next = _next;
		}
	};

	/* Service Queue private data members */
	qNode *front;  // pointer for first node in doubly-linked list of the queue (or null if list is empty)
	qNode *tail;
	int size;

	unQueuedStack *stackHead;

	vector<qNode*> vecPointer; //vector of pointers


public:

	/**
	 * Constructor
	 * Description: intializes an empty service queue.
	 *
	 * RUNTIME REQUIREMENT: O(1)
	 *
		 * TODO
	 */
	ServiceQueue() {
		front = NULL;
		tail = NULL;
		size = 0;

		stackHead = NULL;
	}

	/**
	 * Destructor
	 * Description:  deallocates all memory assciated
	 *   with service queue
	 *
	 * RUNTIME REQUIREMENT:  O(N_b) where N_b is the number of buzzer
	 *	IDs that have been used during the lifetime of the
	 *	service queue; in general, at any particular instant
	 *	the actual queue length may be less than N_b.
	 *
	 *	[See discussion of "re-using buzzers" below]
	 *
		 * TODO
	 */
	~ServiceQueue() {
		clear();
	}

	/**
	* func: clear
	* desc: Deletes all Nodes currently in the list
	* status:  TODO
	*/
	void clear() {
		/*Node *p, *pnxt;
		p = front;
		while (p != nullptr) {
			pnxt = p->next;
			delete p;
			p = pnxt;
		}
		_size = 0;
		front = nullptr;*/
	}

	/**
	 * Function: snapshot()
		 * param:  buzzers is an integer vector passed by ref
	 * Description:  populates buzzers vector with a "snapshot"
		 *               of the queue as a sequence of buzzer IDs
		 *
	 *
	 * RUNTIME REQUIREMENT:  O(N)  (where N is the current queue
	 *		length).
	 */
	void snapshot(std::vector<int> &buzzers) {
		buzzers.clear();   // you don't know the history of the 
						   //   buzzers vector, so we had better
						   //   clear it first.

		qNode* temp = NULL; //temporary pointer
		temp = front;
		while (temp != NULL) {
			if (temp->inQueue == true) {
				buzzers.push_back(temp->buzzNum);
			}
			temp = temp->next;
		}

	}

	/**
	 * Function: length()
	 * Description:  returns the current number of
	 *    entries in the queue.
	 *
	 * RUNTIME REQUIREMENT:  O(1)
	 */
	int  length() {
		return size; //idk chcek this thing
	}

	/**
	 * Function: give_buzzer()
		 * Return:   buzzer-ID (integer) assigned to the new customer.
	 * Description:  This is the "enqueue" operation.  For us
	 *    a "buzzer" is represented by an integer (starting
	 *    from zero).  The function selects an available buzzer
	 *    and places a new entry at the end of the service queue
	 *    with the selected buzer-ID.
	 *    This buzzer ID is returned.
	 *    The assigned buzzer-ID is a non-negative integer
	 *    with the following properties:
	 *
	 *       (1) the buzzer (really it's ID) is not currently
	 *         taken -- i.e., not in the queue.  (It
	 *         may have been in the queue at some previous
	 *         time -- i.e., buzzer can be re-used).
	 *	  This makes sense:  you can't give the same
	 *	  buzzer to two people!
	 *
	 *       (2) Reusable Buzzers:  A re-usable buzzer is
	 *	  a buzzer that _was_ in the queue at some previous
	 *	  time, but currently is not.
	 *
		 *         REQUIREMENT:  If there is one or more reusable
		 *         buzzer, you MUST return one of them; furthermore,
		 *         it must be the buzzer that became reusable most
		 *         MOST RECENTLY.
	 *
	 *       (3) if there are no previously-used / reusable buzzers,
		 *         the SMALLEST possible buzzer-ID is used (retrieved from
		 *         inventory).
	 *	    Properties in this situation (where N is the current
	 *	      queue length):
	 *
	 *		- The largest buzzer-ID used so far is N-1
	 *
	 *		- All buzzer-IDs in {0..N-1} are in the queue
	 *			(in some order).
	 *
	 *		- The next buzzer-ID (from the basement) is N.
	 *
	 *    In other words, you can always get more buzzers (from
	 *    the basement or something), but you don't fetch an
	 *    additional buzzer unless you have to (i.e., no reusable buzzers).
	 *
	 * Comments/Reminders:
	 *
	 *	Rule (3) implies that when we start from an empty queue,
	 *	the first buzzer-ID will be 0 (zero).
	 *
	 *	Rule (2) does NOT require that the _minimum_ reuseable
	 *	buzzer-ID be used.  If there are multiple reuseable buzzers,
	 *	any one of them will do.
	 *
	 *	Note the following property:  if there are no re-useable
	 *	buzzers, the queue contains all buzzers in {0..N-1} where
	 *       N is the current queue length (of course, the buzzer IDs
	 *	may be in any order.)
	 *
	 * RUNTIME REQUIREMENT:  O(1)  ON AVERAGE or "AMORTIZED"
	 *          In other words, if there have been M calls to
	 *		give_buzzer, the total time taken for those
	 *		M calls is O(M).
	 *
	 *		An individual call may therefore not be O(1) so long
	 *		as when taken as a whole they average constant time.
	 *
	 */
	int give_buzzer() {

		if (stackHead == NULL) {
			//case for fresh first buzzer
			//so that means tail is going to change but front will still point to the first one
			qNode* buzzer = new qNode; //make a new Node
			buzzer->buzzNum = size; //if all buzzer from outQueue are used up
			buzzer->inQueue = true;
			buzzer->next = NULL;

			if ((front == NULL) && (tail == NULL)) {
				front = buzzer; //the buzzer will in the beginning
				tail = buzzer; //the buzzer will go at the end
				buzzer->prev = NULL; //if this is the first one
			}
			else {
				tail->next = buzzer; //add new buzzer after the tail
				buzzer->prev = tail; //prev is the tail so yeah
				tail = tail->next; //traverse tail down to the new buzzer
			}

			vecPointer.push_back(buzzer);
			size++; //increase size of the queue
			return size - 1;
		}
		else { //if stackHead is not empty and there are leftover unqueued items
			qNode* temp; //temporary pointer
			temp = vecPointer.at(stackHead->buzzNum); //grab buzzer number from front of the stack
			temp->inQueue = true; //number index is set as inQueue
			size++; //increment size by 1
			tail->next = temp; //added after the tail of queue
			temp->prev = tail; //prev is the tail before so yeah
			tail = tail->next; //new tail is set
			tail->next = NULL; //its at the end so next is null (queue)

			stackHead = stackHead->next; //traverse stackHead down

			return temp->buzzNum;
		}

		return -1; //placeholder
		
	}

	/**
	 * function: seat()
	 * description:  if the queue is non-empty, it removes the first
	 *	 entry from (front of queue) and returns the
	 *	 buzzer ID.
	 *	 Note that the returned buzzer can now be re-used.
	 *
	 *	 If the queue is empty (nobody to seat), -1 is returned to
	 *	 indicate this fact.
		 *
		 * Returns:  buzzer ID of dequeued customer, or -1 if queue is empty.
	 *
	 * RUNTIME REQUIREMENT:  O(1)
	 */
	int seat() {

		if (size == 0) {
			return -1; //queue is empty
		}

		if (front != NULL) {
			int buzzNum;

			//Working with unQueued
			unQueuedStack* buzzer = new unQueuedStack;
			buzzer->buzzNum = front->buzzNum; //get the buzzer number of first in queue
			buzzNum = front->buzzNum; //store for returning

			if (stackHead == NULL) { //if no element in stack
				stackHead = buzzer; //just adding the buzzer to top
				buzzer->next = NULL;
			}
			else {
				buzzer->next = stackHead; //next of new buzzer is old stackhead
				stackHead = buzzer; //new stackhead is now the new buzzer
			}

			//working with queued
			front->inQueue = false; //declare first buzzer number as not in queue
			if (front->next != NULL) {//if there are more than one buzzer
				front = front->next; //now the front is the next buzzer
			}
			
			size--; //reduce the length by one

			return buzzNum;
		}

		return -1;  // placeholder
	}


	/**
	 * function: kick_out()
	 *
	 * description:  Some times buzzer holders cause trouble and
	 *		a bouncer needs to take back their buzzer and
	 *		tell them to get lost.
	 *
	 *		Specifially:
	 *
	 *		If the buzzer given by the 2nd parameter is
	 *		in the queue, the buzzer is removed (and the
	 *		buzzer can now be re-used) and 1 (one) is
	 *		returned (indicating success).
	 *
	 * Return:  If the buzzer isn't actually currently in the
	 *		queue, the queue is unchanged and false is returned
	 *		(indicating failure).  Otherwise, true is returned.
	 *
	 * RUNTIME REQUIREMENT:  O(1)
	 */

	//error
	//not adding values to stack and when give_buzzer() size is incremented but then no values are added to vector
	bool kick_out(int kickBuzzer) {

		if (vecPointer.at(kickBuzzer)->inQueue == true) { //if the kick buzzer is in queue

			//working with stack of unqueued
			unQueuedStack* temp = new unQueuedStack;
			temp->buzzNum = kickBuzzer;

			if (stackHead == NULL) { //if no element in stack
				stackHead = temp; //just adding the buzzer to top
				temp->next = NULL;
			}
			else {
				temp->next = stackHead; //next of new buzzer is old stackhead
				stackHead = temp; //new stackhead is now the new buzzer
			}

			//working with queue
			qNode* tempKickBuzzer = vecPointer.at(kickBuzzer); //temp pointer
			tempKickBuzzer->inQueue = false; //not in the queue anymore so false

			if (size == 1) { //if there is only one node then front and tail are set loose
				front = NULL;
				tail = NULL;
			}
			else {
				if (front->buzzNum == kickBuzzer) { //if kickbuzzer is first in queue
					front = front->next; //traverse front down
					front->prev = NULL; //prev of front is NULL duhh
				}
				else {
					tempKickBuzzer->prev->next = tempKickBuzzer->next; //bridge over the tempKickBuzzer

					if (tempKickBuzzer->next != NULL) {
						tempKickBuzzer->next->prev = tempKickBuzzer->prev;
					}

					tempKickBuzzer->next = NULL; //take it out of the node list
					tempKickBuzzer->prev = NULL; //cut ties
				}
			}
			size--; //length is decreased by 1

			return true;
		}

		//three case one if there is only one, more than one and empty

		return false;  // the buzzer number is not in queue

	}

	/**
	 * function:  take_bribe()
	 * description:  some people just don't think the rules of everyday
	 *		life apply to them!  They always want to be at
	 *		the front of the line and don't mind bribing
	 *		a bouncer to get there.
	 *
	 *	        In terms of the function:
	 *
	 *		  - if the given buzzer is in the queue, it is
	 *		    moved from its current position to the front
	 *		    of the queue.  1 is returned indicating success
	 *		    of the operation.
	 *		  - if the buzzer is not in the queue, the queue
	 *		    is unchanged and 0 is returned (operation failed).
	 *
	 * Return:  If the buzzer isn't actually currently in the
	 *		queue, the queue is unchanged and false is returned
	 *		(indicating failure).  Otherwise, true is returned.
	 *
	 * RUNTIME REQUIREMENT:  O(1)
	 */
	bool take_bribe(int bribeBuzzer) {

		//working with queue
		qNode* tempBribeBuzzer = vecPointer.at(bribeBuzzer); //temp pointer

		if (size == 1) { //if there is only one node then front and tail are set loose
			front = NULL;
			tail = NULL;
		}
		else {
			if (front->buzzNum == bribeBuzzer) { //if kickbuzzer is first in queue
				return true; //do nothing
			}
			else {
				tempBribeBuzzer->prev->next = tempBribeBuzzer->next; //bridge over the tempKickBuzzer

				if (tempBribeBuzzer->next != NULL) {
					tempBribeBuzzer->next->prev = tempBribeBuzzer->prev;
				}

				tempBribeBuzzer->next = front; //add it to the front
				tempBribeBuzzer->prev = NULL; //cut ties
				front->prev = tempBribeBuzzer; //front is now second
				front = tempBribeBuzzer; //new front
			}
		}

		return false;  // placeholder
	}



};   // end ServiceQueue class

#endif