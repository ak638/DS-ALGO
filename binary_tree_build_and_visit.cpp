#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

#include <stack>

struct TreeNode {
	int val;
	TreeNode *left;
	TreeNode *right;
	TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};

enum VisitMode {
	VISIT_PREORDER = 0,
	VISIT_INORDER,
	VISIT_POSTORDER
};

static bool g_build_tree_fail = false;

TreeNode *BuildTreeInner(const int preorder[], int preleft, int preright,
						 const int inorder[], int inleft, int inright)
{
	if (g_build_tree_fail) return NULL;

	// preorder的第一个节点为根节点，然后再inorder里面找根节点，就可以分成左右子树，递归执行便可建立整棵树
	int root_val = preorder[preleft];

	int root_at_inorder_idx = inleft;
	while (root_at_inorder_idx <= inright && inorder[root_at_inorder_idx] != root_val) ++root_at_inorder_idx;
	//TODO
	if (root_at_inorder_idx > inright) 
	{
		g_build_tree_fail = true;
		//printf("exception! %d %d\n", root_at_inorder_idx, inright);
		return NULL;
	}
	
	// 建立根节点
	TreeNode *root =  new TreeNode(root_val);

	// 递归处理划分的左右子树
	// 1 [2 4 7] [3 5 6 8]
	// [4 7 2] 1 [5 3 8 6]
	int left_child_size = root_at_inorder_idx - inleft;
	int right_child_size = inright - root_at_inorder_idx;
	
	// 注意preorder的下标计算不能直接依赖root_at_inorder_idx
	if (left_child_size > 0) root->left = BuildTreeInner(preorder, preleft+1, preleft+left_child_size, inorder, inleft, root_at_inorder_idx-1);
	if (right_child_size > 0) root->right = BuildTreeInner(preorder, preleft+1+left_child_size, preright, inorder, root_at_inorder_idx+1, inright);
	
	return root;
}

TreeNode *BuildFromVisit(const int preorder[], const int inorder[], int n)
{
	// 从前序和中序遍历重建二叉树
	// 假设每个节点的val都是唯一的
	// perorder: 1 2 4 7 3 5 6 8
	// inorder:  4 7 2 1 5 3 8 6

	if (!preorder || !inorder || n <= 0) return NULL;

	TreeNode *root = BuildTreeInner(preorder, 0, n-1, inorder, 0, n-1);

	return root;
}

void VisitNode(const TreeNode *cur)
{
	if (! cur)
	{
		printf("NULL");
		return;
	}
	printf("%d ", cur->val);
}

// 递归遍历
void VisitTree_Recursive(const TreeNode *root, void (*visit_func)(const TreeNode *), int visit_mode = VISIT_PREORDER)
{
	if (! root || !visit_func) return;
	
	if (VISIT_PREORDER == visit_mode) visit_func(root);

	if (root->left) VisitTree_Recursive(root->left, visit_func, visit_mode);

	if (VISIT_INORDER == visit_mode) visit_func(root);

	if (root->right) VisitTree_Recursive(root->right, visit_func, visit_mode);

	if (VISIT_POSTORDER == visit_mode) visit_func(root);
}

// 非递归遍历实现
void VisitTree_PreOrder(const TreeNode *root, void (*visit_func)(const TreeNode *))
{
	// 前序遍历
	// 可以利用类似的思路，利用第二个栈，把push次序逆过来，就形成了后序遍历
	// 见 VisitTree_PostOrder_v2
   	 
	if (!root || !visit_func) return;

	const TreeNode *cur = NULL;

	std::stack<const TreeNode *> st;
	st.push(root);

	while (!st.empty())
	{
		cur = st.top();
		st.pop();

		visit_func(cur);

		// 先push右节点
		// 这是为了保证左节点在下次循环首先被pop出来处理，
		// 同时也被左节点的子节点覆盖在上面，满足处理完左子树，
		// 再处理右子树的特点
		if (cur->right) st.push(cur->right);
		
		if (cur->left) st.push(cur->left);
	}
}

void VisitTree_InOrder(const TreeNode *root, void (*visit_func)(const TreeNode *))
{
	// 中序遍历
   	 
	if (!root || !visit_func) return;

	const TreeNode *cur = NULL;

	std::stack<const TreeNode *> st;
	st.push(root);

	while (!st.empty())
	{
		// 向左走到尽头
		while (st.top()->left) st.push(st.top()->left);
		
		// 不断退栈，访问节点，试图找到右出路
		do
		{
			cur = st.top();
			st.pop();
			visit_func(cur);

			// 右结点存在，切换成外循环走左
			if (cur->right)
			{
				st.push(cur->right);
				break;
			}

		} while (!st.empty());
	}

}

void VisitTree_PostOrder(const TreeNode *root, void (*visit_func)(const TreeNode *))
{
	// 后序遍历
   	 
	if (!root || !visit_func) return;

	const TreeNode *cur = NULL;
	// pre指向上次处理的结点，以便判断从左/右子树处理完成回到当前结点
	const TreeNode *pre = NULL;

	std::stack<const TreeNode *> st;
	st.push(root);
	
	while (!st.empty())
	{
		// 向左走到尽头
		while (st.top()->left) st.push(st.top()->left);

		// 不断退栈，判断从右子树返回再访问节点，不满足访问条件，
		// 把右结点加入栈，走外循环
		do
		{
			pre = cur;
			cur = st.top();

			// 右结点不存在或者上次已经处理了右结点，表明当前结点可以退栈和处理了
			if (cur->right == NULL || cur->right == pre) 
			{
				st.pop();
				visit_func(cur);
				continue;
			}

			// 到了这里，表明右子树存在，并且还没被访问
			// push右结点，走外循环逻辑
			st.push(cur->right);
			break;

		} while (!st.empty());
	}

}

void VisitTree_PostOrder_v2(const TreeNode *root, void (*visit_func)(const TreeNode *))
{
	// 后序遍历 双栈法
	// 思路类似前序遍历的非递归法

	// 顺序栈访问顺序：根->右->左，对逆序栈，则是左->右->根,
	// 正是后序遍历需要的顺序
   	 
	if (!root || !visit_func) return;

	const TreeNode *cur = NULL;

	std::stack<const TreeNode *> st;
	st.push(root);

	std::stack<const TreeNode *> st_back;

	while (!st.empty())
	{
		cur = st.top();
		st.pop();

		// push倒序栈
		st_back.push(cur);

		// 后push右，保证右先被处理
		if (cur->left) st.push(cur->left);
		
		if (cur->right) st.push(cur->right);
	}

	while (!st_back.empty())
	{
		visit_func(st_back.top());
		st_back.pop();
	}
}


int main(int argc, char *argv[])
{
	freopen("./input.txt", "r", stdin);

	int n;
	scanf("%d", &n);

	int *preorder = new int[n];
	int *inorder = new int[n];

	for (int i = 0; i < n; ++i) scanf("%d", &preorder[i]);
	for (int i = 0; i < n; ++i) scanf("%d", &inorder[i]);

	TreeNode *root = BuildFromVisit(preorder, inorder, n);
	printf("BuildFromVisit ret %d\n", g_build_tree_fail);

	VisitTree_Recursive(root, VisitNode);
	printf("(recursive preoder)\n");
	VisitTree_Recursive(root, VisitNode, VISIT_INORDER);
	printf("(recursive inorder)\n");
	VisitTree_Recursive(root, VisitNode, VISIT_POSTORDER);
	printf("(recursive postorder)\n");

	VisitTree_PreOrder(root, VisitNode);
	printf("(nonrecursive preoder)\n");

	VisitTree_InOrder(root, VisitNode);
	printf("(nonrecursive inorder)\n");
	
	VisitTree_PostOrder(root, VisitNode);
	printf("(nonrecursive postorder)\n");

	VisitTree_PostOrder_v2(root, VisitNode);
	printf("(nonrecursive postorder v2)\n");

	return 0;
}

/*
   test case:
8
1 2 4 7 3 5 6 8
4 7 2 1 5 3 8 6
*/
