#include <AppElement.hpp>


AppElement::AppElement(Context* cntx)
	: mContext(cntx) {
}

Context* AppElement::context() {
	return mContext;
}

void AppElement::setContext(Context* cntx) {
	mContext = cntx;
}
