#include <cv.h>
#include <highgui.h>

struct Rectangle
{
	int left, top, width, height;

	Rectangle()
	{
	}

	Rectangle(int x, int y, int w, int h)
	{
		left = x;
		top = y;
		width = w;
		height = h;
	}
	bool fitIn(const Rectangle& container) const
	{
		return width <= container.width && height <= container.height;
	}

	bool fitExactlyIn(const Rectangle& container) const
	{
		return width == container.width && height == container.height;
	}
};


void drawRectangle(cv::Mat& mask, Rectangle rc)
{
	auto color = cv::Scalar(rand() % 255, rand() % 255, rand() % 255);
	auto thickness = 5;
	cv::line(mask, cv::Point(rc.left, rc.top), cv::Point(rc.left, rc.top + rc.height - 1), color,thickness);
	cv::line(mask, cv::Point(rc.left, rc.top), cv::Point(rc.left + rc.width - 1, rc.top), color, thickness);
	cv::line(mask, cv::Point(rc.left + rc.width - 1, rc.top), cv::Point(rc.left + rc.width - 1, rc.top + rc.height - 1), color, thickness);
	cv::line(mask, cv::Point(rc.left, rc.top + rc.height - 1), cv::Point(rc.left + rc.width - 1, rc.top + rc.height - 1), color, thickness);
}

struct Node
{
	Node* child[2];
	Rectangle rc;
	int imageID;
	
	Node()
	{
		imageID = -1;
		child[0] = NULL;
		child[1] = NULL;
	}

	Node* Insert(const Rectangle& img,cv::Mat& mask)
	{
		//if this is not leaf node(empty)
		if (imageID != -1)
		{
			//(try inserting into first child)
			if (child[0] == NULL)
				return NULL;
			Node* newNode = child[0]->Insert(img,mask);
			if (newNode != NULL)
				return newNode;

			//	(no room, insert into second)
			return child[1]->Insert(img,mask);
		}
		else // leaf node
		{

			//(if there's already a lightmap here, return)
			if (imageID != -1)
				return NULL;

			//(if we're too small, return)
			//if (img doesn't fit in pnode->rect)
			if (!img.fitIn(rc))
				return NULL;

			// //Node* pnode = new Node;
			// //this->rc = rc;
			this->imageID = rand();
			//(if we're just right, accept)
			if (img.fitExactlyIn(rc))
			{
				drawRectangle(mask, this->rc);
				std::cout << rc.left << " " << rc.top << " " << rc.width << " " << rc.height << "\n";
				return this;
			}
			//(otherwise, gotta split this node and create some kids)
			this->child[0] = new Node;
			this->child[1] = new Node;

			//(decide which way to split)
			int dw = rc.width - img.width;
			int dh = rc.height - img.height;

			if (dw > dh)
			{
				child[0]->rc = Rectangle(rc.left, rc.top,
					img.width, rc.height);
				child[1]->rc = Rectangle(rc.left + img.width, rc.top,
					dw, rc.height);
			}
			else
			{
				child[0]->rc = Rectangle(rc.left, rc.top,
					rc.width, img.height);
				child[1]->rc = Rectangle(rc.left, rc.top + img.height,
					rc.width, dh);
			}

			//(insert into first child we created)
			return this->child[0]->Insert(img,mask);//Insert(img, this->child[0]);
		}
	}
};



int main()
{
	Node rootnode;
	rootnode.rc = Rectangle(0, 0, 400, 400);
	cv::Mat mask = cv::Mat(rootnode.rc.height, rootnode.rc.width, CV_8UC3);
	rootnode.Insert(Rectangle(0, 0, 100, 200),mask);
	rootnode.Insert(Rectangle(0, 0, 300, 100), mask);
	rootnode.Insert(Rectangle(0, 0, 50, 100), mask);
	rootnode.Insert(Rectangle(0, 0, 100, 50), mask);
	rootnode.Insert(Rectangle(0, 0, 50, 100), mask);
	rootnode.Insert(Rectangle(0, 0, 100, 50), mask);
	rootnode.Insert(Rectangle(0, 0, 50, 100), mask);
	rootnode.Insert(Rectangle(0, 0, 100, 50), mask);
	rootnode.Insert(Rectangle(0, 0, 50, 100), mask);
	rootnode.Insert(Rectangle(0, 0, 100, 50), mask);
	rootnode.Insert(Rectangle(0, 0, 200, 200), mask);
	rootnode.Insert(Rectangle(0, 0, 100, 50), mask);
	rootnode.Insert(Rectangle(0, 0, 50, 100), mask);
	rootnode.Insert(Rectangle(0, 0, 100, 50), mask);
	rootnode.Insert(Rectangle(0, 0, 50, 100), mask);
	rootnode.Insert(Rectangle(0, 0, 100, 50), mask);
	cv::imshow("", mask);
	cv::waitKey();
	system("pause");
	return 0;
}