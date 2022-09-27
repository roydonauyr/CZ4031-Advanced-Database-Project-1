Insert(key, ptr){
    if (root == null){
        //Create new root
        //insert key
        //insert ptr
        return
    }
		
	else{
        *currNode = root

		while(currNode -> type != 2){
			For (int i=0; i< currNode->getlength(); i++):
				if(key < currNode -> key[i]){
					currNode = currNode -> ptr[i]
					break;
				}

				If (i == ->getlength()-1){
					currNode = currNode -> ptr[i+1]
					break;
                }

        }

        //Now i am at the correct spot for insertion
        //Check if there is still space for insertion
        if (currNode->getlength() < currNode->key.size()){
            Int i = 0
            While (key > currNode -> key[i] && i < currNode->key.size()){
                i++;
            }
                
            if (currNode -> key[i] == key){

                //Check if it is pointing to a linkedlist block if not must creat
                if(currNode -> ptr[i] -> type == 3){
                    if(linkedList block is full){
                        //create new block
                        //Push into linkedList block
                    }else{
                        //Push into linkedList block
                    }
                    return
                }
                //create linkedList block
                //push curr record address in linkedlist block
                //push new record address in linkedList block
                //2 since the below code would override the curr record address (ptr[i])
                ptr[i] = linkedListblock address;
                return
            }

            //shift all keys back to insert current key at the index
            For (int j = currNode-> key.size(); j > i; j--){
                currNode->key[j] = currNode -> key[j-1]
                currNode->ptr[j] = currNode ->ptr[j-1]
            }
                
            //insert new key
            currNode -> key[i] = key
            currNode -> ptr[i] = ptr
         
        }else{
            //First check if duplicate
            Int i = 0
            while (key > currNode -> key[i] && i < currNode->key.size()){
                i++;
            }
                
            if (currNode -> key[i] == key){

                //Check if it is pointing to a linkedlist block if not must creat
                if(currNode -> ptr[i] -> type == 3){
                    if(linkedList block is full){
                        //create new block
                        //Push into linkedList block
                    }else{
                        //Push into linkedList block
                    }
                    return
                }
                //create linkedList block
                //push curr record address in linkedlist block
                //push new record address in linkedList block
                //2 since the below code would override the curr record address (ptr[i])
                ptr[i] = linkedListblock address;
                return
            }


            // No space hence need to split node
            // create temp list and increase space for insert
            tempkey[currNode->getLength()+1];
            //temp ptr does not include last ptr to next leaf node hence same length as keys
            tempPtr[currNode->getLength()+1];
            
            for (int k= 0; k< i; k++){
                tempkey[k] = currNode-> key[k]
                tempPtr[k] = currNode-> ptr[k]
            }

            // i is where your key should be inserted calculated above
            tempkey[i] = key
            tempPtr[i] = ptr

            // k-1 since starting from where we left off in currNode which is at i
            for (int k = i+1; k<currNode->getLength()+1; k++){
                tempkey[k] = currNode-> key[k-1]
                tempPtr[k] = currNode-> ptr[k-1]
            }

            // Create a new leaf node
            Node newleaf..... 
            set type as 1
            // Pad everything as 0 when create

            // clear curNode keys and ptr
            for (int k = 0; k<currNode->key.size(); k++){
                currNode->key[k] = 0
                currNode->Ptr[k] = null or 0
            }
            // Set last ptr of currenode to the new leaf node
            //Save the next ptr in case it is pointing to another leaf node
            //Then the new leaf would have to point to this node
            nextNodePtr = curNode->Ptr[currNode->key.size()]
            currNode->Ptr[currNode->key.size()] = address of newleaf

            
            


            //tempkey already accounts for +1
            //currNode should take ceiling of (max keys + 1) / 2
            // new node should take floor of (max keys + 1) / 2
            //must import library cmath
            currNodetakes = ceil(tempkey.size()/2)
            
            for (int k = 0; k<currNodetakes; k++){
                currNode->key[k] =  tempkey[k]
                currNode->ptr[k] =  tempPtr[k]
            }

            for (int k = currNodetakes; k< tempkey.size(); k++){
                newleaf->key[k-currNodetakes] =  tempkey[k]
                newleaf->ptr[k-currNodetakes] =  tempPtr[k]
            }

            //set new leaf ending pointer to point to next ndoe
            newLeaf -> ptr[newLeaf->key.size()] = nextNodePtr;
            

            if(currNode == root){
                //create a new root
                Node Root

                // LB of root
                Root-> key[0] = newLeaf -> key[0]

                Root-> ptr[0] = currNode;
                Root-> ptr[1] = newLeaf;

                Root->type = 1;
                //Save new root address
                root = Root;
                
                //Update parent address
                currNode parent address = root
                newLeaf parent address = root
            }else{
                //Recursive Call for insert in internal
                insertInternal(newLeaf->key[0], currNode parent address, newLeaf);
            }

        }		




    }		
}

//current node is now parent
void insertInternal(key, Node *currNode, Node *child){
    
    //if parent (current node) still has space, we can add the child node as a pointer
    if (currNode->getLength() < currNode->key.size()){
        int i = 0, j;

        while(key > currNode->key[i] && i < currNode->getLength()){
            i++;
        }

        // using index i, move larger keys backwards
        for (j = currNode -> key.size(); j > i; j--){
            currNode->key[j] = currNode->key[j-1]
        }

        //shift pointers right, i+1 because ptr is now on right of key
        for (j = currNode->key.size()+1; j>i+1; j--){
            cuurNode->ptr[j] = currNode->ptr[j-1]
        }

        //add in new child lower bound key and pointer to parent
        currNode->key[i] = key;
        currNode->ptr[i+1] = child;

        //update parent address for child (Which is new leaf)
        child->parent = currNode

    }else{ // If parent node no more space, recursively split parent node

        // No space hence need to split node
        // create temp list and increase space for insert
        tempkey[currNode->getLength()+1];
        //temp ptr does not include last ptr to next leaf node hence same length as keys
        tempPtr[currNode->getLength()+1];

        int i = 0
        while (key > currNode -> key[i] && i < currNode->key.size()){
            i++;
        }

        for (int k= 0; k< i; k++){
            tempkey[k] = currNode-> key[k]
        }

        for (int k= 0; k< i+1; k++){
            tempPtr[k] = currNode-> ptr[k]
        }

        tempkey[i] = key
        tempPtr[i+1] = child

        // to be used later at ceiling to update its parent
        // depending on the node split
        child_pos = i+1

        // k-1 since starting from where we left off in currNode which is at i
        for (int k = i+1; k<currNode->getLength()+1; k++){
            tempkey[k] = currNode-> key[k-1]
        }

        for (int k = i+2; k<currNode->getLength()+2; k++){
            tempPtr[k] = currNode-> ptr[k-1]
        }

        // Create a new node
        Node newInternal..... 
        set type as 1

        // clear curNode keys and ptr
        for (int k = 0; k<currNode->key.size(); k++){
            currNode->key[k] = 0
        }

        for (int k = 0; k<currNode->key.size()+1; k++){
            currNode->Ptr[k] = null or 0
        }

        //NOT SURE ON THIS SPLIT BECAUSE TECHNICALLY INTERNAL NODE
        //NEW NODE SHOULD BE ONE LESS SINCE AFTER SPLIT
        //EG: 6 KEYS, 3 KEYS On LEFT, only 2 on the right since 
        // SHOULD be 7 pointers, 4 on the left, 3 on the right
        // SO SHOULD WE DROP THE FIRST KEY ON THE NEW NODE SINCE NOT NEEDED?
        //currNode should take ceiling of (max keys + 1) / 2
        // new node should take floor of (max keys + 1) / 2
        //must import library cmath
        currNodetakes = ceil(currNode->key.size()/2)
        
        for (int k = 0; k<currNodetakes; k++){
            currNode->key[k] =  tempkey[k]
        }

        for (int k = 0; k<currNodetakes+1; k++){
            currNode->ptr[k] =  tempPtr[k]
        }

        for (int k = currNodetakes; k< tempkey.size(); k++){
            newInternal->key[k-currNodetakes] =  tempkey[k]
        }

        for (int k = currNodetakes+1; k< tempPtr.size(); k++){
            newInternal->ptr[k-currNodetakes-1] =  tempPtr[k]
        }

        // Update childs parent first
        if (child_pos <= currNodetakes-1){
            child-> parent address = currNode
        }else{
            child-> parent address = newInternal
        }


        // Check if current node is root node
        // If it is need to create a new root
        if(currNode == root){
                //create a new root
                Node Root

                // LB of root
                Root-> key[0] = newInternal -> key[0]

                Root-> ptr[0] = currNode;
                Root-> ptr[1] = newInternal;

                Root->type = 1;
                //Save new root address
                root = Root;
                
                //Update parent address
                currNode parent address = root
                newInternal parent address = root
        }else{
            //Recursive Call for insert in internal

            //To find the key to pass in (LB of the child node)
            temp = newInternal
            while(temp -> type != 1){ // type leaf node
                temp = temp->Ptr[0]
            }
            key = temp->key[0]
            insertInternal(key, currNode parent address, newInternal);
        }


    }

}


	
	
