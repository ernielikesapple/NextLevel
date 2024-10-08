#include <iostream>
#include <memory>
using namespace std;
struct TreeNode {
    // member value
    int nodeValue;   // the data type can be anything 
    unique_ptr<TreeNode> leftNode; // declare unique pointer to automactically manage the memory
    unique_ptr<TreeNode> rightNode;
    
    // constructor 
    // TreeNode(int value) {
    //     nodeValue = value;
    //     leftNode = nullptr;
    //     rightNode = nullptr;
    // }

    // or we can write above code into one line
    TreeNode(int value):  nodeValue(value), leftNode(nullptr),rightNode(nullptr) {}
};

// in order to create an actual tree with root node so here we create a class of BSTree
// template <typename T>   
class BSTree {
    public:
    // constructor inside:
        // BSTree () {
        //     rootAddress = nullptr;
        // }
        BSTree(): rootAddress(nullptr) {}
        
        // BSTree (TreeNode* rootNode) {
        //     rootAddress = rootNode;
        // }
        BSTree (unique_ptr<TreeNode> rootNode): rootAddress(std::move(rootNode)) {}
        // whenever we need to assign the pointer a new value use move function to modify the actual content directly
        void Print() const;   // to print the tree as string, each node is "{ nodeValue }"
        void Insert(const int value); // insert a node to the BST
        // insert the actual value not a node, in order not to create a new one
    // void Print() {
    //     if (this->rootAddress == nullptr) {
    //         cout << "{}" << endl;
    //     } else {
    //         cout << this->PrintTreeNode(this->rootAddress) << endl; // this key word is pointer to the class so we need  -> to access the class methods
    //     }
    // }
    // we can define the class outside the class as well
        bool Contains(const int value) const;
        // this method only does not need to modify the original node so pass by a const reference  
        
        void Remove(int value); // remove the generic T type, and only add value as parameter to avoid create another new node in the main
    
    private:
        unique_ptr<TreeNode> rootAddress; // private class member can only access through public method 
        string PrintTreeNode(const TreeNode* node) const;
        // in Print there is no need to modify the ownership of TreeNode so no need pass the unique_ptr, so just use a const raw pointer
        void InsertRecursiveForSubTree(const int value, unique_ptr<TreeNode>& currentRoot);
        
        bool ContainsRecursiveForSubTree(const int value, const unique_ptr<TreeNode> &root) const;
        
        void RemoveRecursiveForSubTree(int value, unique_ptr<TreeNode> &currentRoot);
        
        auto FindMinSubTree(unique_ptr<TreeNode>& currentRoot) -> int;
};

void BSTree::Print() const {
    cout << "Print the whole tree as string ";
    if (this->rootAddress == nullptr) {
        cout << "{}" << endl;
    } else {
        cout << this->PrintTreeNode(this->rootAddress.get()) << endl; // this key word is pointer to the class so we need  -> to access the class methods
        // .get() method to return a raw pointer from the unique_ptr
    }
}

string BSTree::PrintTreeNode(const TreeNode* node) const {
    string leftNodeString = (node->leftNode!=nullptr) ? this->PrintTreeNode(node->leftNode.get()) : "{}";  // we need to pass the child do the recussion
    string rightNodeString = node->rightNode ? this->PrintTreeNode(node->rightNode.get()) : "{}"; // notice the condition order can also be just the variable itself (node->rightNode) to check if it exists
    string result = "{" + to_string(node->nodeValue) + "," + leftNodeString + "," + rightNodeString + "}"; // iterate the tree node using the order ‘left, root, right’, this order can also be 'root, left, right' or 'left, right, root'
    return result;
}

void BSTree::Insert(const int value) {
    if (this->rootAddress == nullptr) { // nullptr is pointer, NULL is an int means 0 comes from C, but it can also use as nullptr in some cases
        auto newNode = make_unique<TreeNode>(value);
        this->rootAddress = std::move(newNode);
    } else {
        this-> InsertRecursiveForSubTree(value, this->rootAddress);
    }
}

void BSTree::InsertRecursiveForSubTree(const int value, unique_ptr<TreeNode>& currentRoot) {
    // since we need to modify the value passing by outside the scope so we need to pass this current root node by reference to modify the original content
    
    if (currentRoot->nodeValue > value) {
        // if the new node value is smaller than the root node we need to insert this new to the left child tree
        if (currentRoot->leftNode == nullptr) {
            // if this node does not have a left node then do the insertion 
            auto newNode = make_unique<TreeNode>(value);
            currentRoot->leftNode = std::move(newNode);
        } else {
            this->InsertRecursiveForSubTree(value, currentRoot->leftNode);
        }
        return;
    } else if(currentRoot->nodeValue == value) {
        return;
    } else if(currentRoot->nodeValue < value) {
        if (currentRoot->rightNode == nullptr) {
            // if this node does not have a left node then do the insertion 
            auto newNode = make_unique<TreeNode>(value);
            currentRoot->rightNode = std::move(newNode);
        } else {
            this->InsertRecursiveForSubTree(value, currentRoot->rightNode);
        }
        return;
    }
}

bool BSTree::Contains(const int value) const {
    if (this->rootAddress == nullptr) {
        return false;
    } else {
        return this->ContainsRecursiveForSubTree(value, this->rootAddress);
    }
}

bool BSTree::ContainsRecursiveForSubTree(const int value, const unique_ptr<TreeNode> &root) const {
    if (root->nodeValue == value) {
        return true; 
    } 
    
    if (root->nodeValue < value) {
        if (root->rightNode == nullptr) {
            return false;
        } else {
            return this->ContainsRecursiveForSubTree(value, root->rightNode);
        }
    } else {
        if (root->leftNode == nullptr) {
            return false;
        } else {
            return this->ContainsRecursiveForSubTree(value, root->leftNode);
        }
    }
}

void BSTree::Remove(const int value) {
    if (this->rootAddress) {
        this->RemoveRecursiveForSubTree(value, this->rootAddress);
    }
}

void BSTree::RemoveRecursiveForSubTree(int value, unique_ptr<TreeNode>& currentRoot) {
    if (currentRoot == nullptr) {
        return;
    }
    
    if (value > currentRoot->nodeValue) {
        this->RemoveRecursiveForSubTree(value, currentRoot->rightNode);
    } else if (value < currentRoot->nodeValue) {
        this->RemoveRecursiveForSubTree(value, currentRoot->leftNode);
    } else {
    // currentRoot->nodeValue == value
    // move function transfer the ownership, when passing by reference, there is no need use move function
        // remove the node 
        if (currentRoot -> leftNode == nullptr && currentRoot -> rightNode == nullptr) {
            currentRoot = nullptr;
        } else if (currentRoot -> leftNode != nullptr && currentRoot -> rightNode == nullptr) {
             currentRoot = std::move(currentRoot->leftNode); // after this process the tree remains balanced since the removed node is the bigger than all the left sub tree, the new linked node still keeps the balance
        } else if (currentRoot -> rightNode != nullptr && currentRoot -> rightNode == nullptr) {
            currentRoot = std::move(currentRoot->rightNode);
        } else {
            // step 1: find  min value of rightSub tree, set that min value node to nullptr 
             auto minValue = this->FindMinSubTree(currentRoot->rightNode);
             // step 2:  change root node value
             currentRoot -> nodeValue = minValue;
        }
        return;
    }    
}

auto BSTree::FindMinSubTree(unique_ptr<TreeNode>& currentRoot) -> int {
    if (currentRoot -> leftNode != nullptr) {
        return this-> FindMinSubTree(currentRoot->leftNode);
    } else {
        auto value = currentRoot->nodeValue;
        if (currentRoot -> rightNode == nullptr) {
            currentRoot = nullptr;    
        } else {
            currentRoot = std::move(currentRoot -> rightNode);
        }
        return value;
    }
}

int main() {
    // Write C++ code here
    
    auto myTreeNode = make_unique<TreeNode>(5); // Root Node, we can use auto let the compiler deduce the type
    // make_unique return a unique_ptr auto deleting pointer manage the object and return a new instance of TreeNode object
    // and the type is not a TreeNode anymore it's a unique_ptr<TreeNode>
    auto myTreeLeftNode = make_unique<TreeNode>(3); // Left Child Node
    auto myTreeRightNode = make_unique<TreeNode>(8); // Right Child Node
    
    // node connection, this is a very important step to build a binary tree using linked list idea
    myTreeNode->leftNode = std::move(myTreeLeftNode); // myTreeNode is a root node so when it accesses the child node it uses . symbol, since it is not a pointer 
    // we need to use & symbol before myTreeLeftNode to pass the address of the Left Child Node to the root node left child pointer
    // move(), transfer the pointer owner ship without copying the original data 
    myTreeNode->rightNode = std::move(myTreeRightNode);
    // cout << myTreeNode.leftNode -> nodeValue  << endl;   since the leftNode variable is a pointer so we need to use -> symbol to access the node value
    cout << "Print a single node value " << myTreeNode->leftNode -> nodeValue  << endl;
    
    // BSTree emptyTree(nullptr);
    BSTree emptyTree {}; // above line can be also in this way, using the  {} to initializing the instance of this class
    emptyTree.Print();
    
    // to print the tree using { {}, nodeValue, {} } this format 
    BSTree threeNodesTree{std::move(myTreeNode)};
    threeNodesTree.Print();
    
    threeNodesTree.Insert(4);
    threeNodesTree.Insert(2);
    threeNodesTree.Insert(24);

    threeNodesTree.Print();
    
    cout << threeNodesTree.Contains(4) << endl; // here you can not pass the nodeFourth to this call again, since after the move method this unique pointer is nullptr
    threeNodesTree.Print();

    threeNodesTree.Remove(4);
    threeNodesTree.Print();
    
    cout << threeNodesTree.Contains(4) << endl;
    
    auto rootTreeNode = make_unique<TreeNode>(3);
    BSTree oneNodeTree{std::move(rootTreeNode)};
    oneNodeTree.Remove(3);
    oneNodeTree.Print();
    

    return 0;
}
