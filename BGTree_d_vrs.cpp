//
// Created by swh on 2022/2/20.
//

#include "BGTree_d_vrs.h"

BGTree_d_vrs::BGTree_d_vrs() {
	numSub = 0;
	numNode = 0;
	height = 0;
	maxNodeSize = MAXNodeSIZE;
//	memset(subPredicate, 0, sizeof(subPredicate));
	roots.resize(atts);
	_for(i, 0, atts) {
		roots[i] = new bluenode_d_vrs(0, valDom - 1, ++numNode, 1, nullptr, nullptr, nullptr, nullptr);
	}
//	nB.resize(atts);
//	nnB.resize(atts);
	predicateNumOnAttr.resize(atts);
	cout << "ExpID = " << expID << ". BGTree_d_vrs: maxNodeSize = " << maxNodeSize << "\n";
}

BGTree_d_vrs::~BGTree_d_vrs() {
	_for(i, 0, atts) {
		releaseBlueNode(roots[i]);
	}
}

void BGTree_d_vrs::releaseBlueNode(bluenode_d_vrs *&r) {
	if (r->lowGreenChild) releaseGreenNode(r->lowGreenChild);
	if (r->highGreenChild) releaseGreenNode(r->highGreenChild);
	if (r->leftBlueChild) releaseBlueNode(r->leftBlueChild);
	if (r->rightBlueChild) releaseBlueNode(r->rightBlueChild);
	delete r;
}

void BGTree_d_vrs::releaseGreenNode(lgreennode_d_vrs *&r) {
	if (r->leftChild) releaseGreenNode(r->leftChild);
	if (r->rightChild) releaseGreenNode(r->rightChild);
	if (r->realBstPtr) delete r->realBstPtr;
	delete r;
}

void BGTree_d_vrs::releaseGreenNode(hgreennode_d_vrs *&r) {
	if (r->leftChild) releaseGreenNode(r->leftChild);
	if (r->rightChild) releaseGreenNode(r->rightChild);
	if (r->realBstPtr) delete r->realBstPtr;
	delete r;
}

void BGTree_d_vrs::calPredicateNum(const vector<IntervalSub> &subList) {
	predicateNumOnAttr.resize(atts, 0);
	for (auto &&iSub: subList) {
		for (auto &&iCnt: iSub.constraints) {
			predicateNumOnAttr[iCnt.att]++;
		}
	}
}

int BGTree_d_vrs::getVirtualAttrByMinFre(const IntervalSub &iSub) {
//	int vIndex=0;
//	for(int i=1;i<iSub.constraints.size();i++){
//		if(predicateNumOnAttr[i]<predicateNumOnAttr[vIndex])
//			vIndex=i;
//	}
//	return vIndex;
	return min_element(iSub.constraints.begin(), iSub.constraints.end(),
					   [&](const IntervalCnt &a, const IntervalCnt &b) {
						   return predicateNumOnAttr[a.att] < predicateNumOnAttr[b.att];
					   })->att;
}

int BGTree_d_vrs::getVirtualAttrByMinWidth(const IntervalSub &iSub) {
	return min_element(iSub.constraints.begin(), iSub.constraints.end(),
					   [](const IntervalCnt &a, const IntervalCnt &b) {
						   return a.highValue - a.lowValue < b.highValue - b.lowValue;
					   })->att;
}

void BGTree_d_vrs::insert(IntervalSub sub, const vector<IntervalSub> &subList) {
	int virtualAttr = getVirtualAttrByMinFre(sub);
	for (auto &&c: sub.constraints) {
		virtualAttr == c.att ?
		insertVAttrIntoBlueNode(roots[c.att], sub.id, c.lowValue, c.highValue, c.att, subList):\
		insertRAttrIntoBlueNode(roots[c.att], sub.id, c.lowValue, c.highValue, c.att, subList);
	}
	numSub++;
}

void BGTree_d_vrs::insertVAttrIntoBlueNode(bluenode_d_vrs *&r, const int &subID, const int &l, const int &h,
										   const int &attrId,
										   const vector<IntervalSub> &subList) {
	r->vSubids.push_back(subID);

	if (r->leftBlueChild != nullptr) { // has child
		if (h <= r->mid) {
			insertVAttrIntoBlueNode(r->leftBlueChild, subID, l, h, attrId, subList);
		} else if (l > r->mid) {
			insertVAttrIntoBlueNode(r->rightBlueChild, subID, l, h, attrId, subList);
		} else {
			insertVAttrIntoGreenNode(r->lowGreenChild, subID, l, attrId, subList);
			insertVAttrIntoGreenNode(r->highGreenChild, subID, h, attrId, subList);
		}
	} else if (r->rSubids.size() > maxNodeSize) { // no child and too big node
		r->leftBlueChild = new bluenode_d_vrs(r->l, r->mid, ++numNode, r->levelid + 1, nullptr, nullptr,
											  nullptr,
											  nullptr);
		r->rightBlueChild = new bluenode_d_vrs(r->mid + 1, r->h, ++numNode, r->levelid + 1, nullptr, nullptr,
											   nullptr, nullptr);
		r->lowGreenChild = new lgreennode_d_vrs(r->l, r->mid, ++numNode, r->levelid + 1, nullptr, nullptr, nullptr);
		r->highGreenChild = new hgreennode_d_vrs(r->mid + 1, r->h, ++numNode, r->levelid + 1, nullptr, nullptr,
												 nullptr);
		for (auto &&id: r->vSubids) {
			for (auto &&c: subList[id].constraints) {
				if (c.att == attrId) {
					if (c.highValue <= r->mid) {
						r->leftBlueChild->vSubids.push_back(id);
					} else if (c.lowValue > r->mid) {
						r->rightBlueChild->vSubids.push_back(id);
					} else { // l<=mid<mid+1<=h
						r->lowGreenChild->vSubids.push_back(id);
						r->highGreenChild->vSubids.push_back(id);
					}
					break;
				}
			}
		}
		for (auto &&id: r->rSubids) {
			for (auto &&c: subList[id].constraints) {
				if (c.att == attrId) {
					if (c.highValue <= r->mid) {
						r->leftBlueChild->rSubids.push_back(id);
						if (r->leftBlueChild->mid == c.highValue) r->leftBlueChild->lMidv.push_back(id);
						else if (r->leftBlueChild->mid + 1 == c.lowValue) r->leftBlueChild->rMidv.push_back(id);
					} else if (c.lowValue > r->mid) {
						r->rightBlueChild->rSubids.push_back(id);
						if (r->rightBlueChild->mid == c.highValue) r->rightBlueChild->lMidv.push_back(id);
						else if (r->rightBlueChild->mid + 1 == c.lowValue) r->rightBlueChild->rMidv.push_back(id);
					} else { // l<=mid<mid+1<=h
						r->lowGreenChild->rSubids.push_back(id);
						if (r->lowGreenChild->mid + 1 == c.lowValue)
							r->lowGreenChild->midv.push_back(id);
						r->highGreenChild->rSubids.push_back(id);
						if (r->highGreenChild->mid - 1 == c.highValue)
							r->highGreenChild->midv.push_back(id);
					}
					break;
				}
			}
		}
	}
}
void BGTree_d_vrs::insertRAttrIntoBlueNode(bluenode_d_vrs *&r, const int &subID, const int &l, const int &h,
										   const int &attrId,
										   const vector<IntervalSub> &subList) {
	r->rSubids.push_back(subID);
	if (r->mid == h) r->lMidv.push_back(subID);
	else if (r->mid + 1 == l) r->rMidv.push_back(subID);

	if (r->leftBlueChild != nullptr) { // has child
		if (h <= r->mid) {
			insertRAttrIntoBlueNode(r->leftBlueChild, subID, l, h, attrId, subList);
		} else if (l > r->mid) {
			insertRAttrIntoBlueNode(r->rightBlueChild, subID, l, h, attrId, subList);
		} else {
			insertVAttrIntoGreenNode(r->lowGreenChild, subID, l, attrId, subList);
			insertRAttrIntoGreenNode(r->highGreenChild, subID, h, attrId, subList);
		}
	} else if (r->rSubids.size() > maxNodeSize) { // no child and too big node
		r->leftBlueChild = new bluenode_d_vrs(r->l, r->mid, ++numNode, r->levelid + 1, nullptr, nullptr,
											  nullptr,
											  nullptr);
		r->rightBlueChild = new bluenode_d_vrs(r->mid + 1, r->h, ++numNode, r->levelid + 1, nullptr, nullptr,
											   nullptr, nullptr);
		r->lowGreenChild = new lgreennode_d_vrs(r->l, r->mid, ++numNode, r->levelid + 1, nullptr, nullptr, nullptr);
		r->highGreenChild = new hgreennode_d_vrs(r->mid + 1, r->h, ++numNode, r->levelid + 1, nullptr, nullptr,
												 nullptr);
		for (auto &&id: r->vSubids) {
			for (auto &&c: subList[id].constraints) {
				if (c.att == attrId) {
					if (c.highValue <= r->mid) {
						r->leftBlueChild->vSubids.push_back(id);
					} else if (c.lowValue > r->mid) {
						r->rightBlueChild->vSubids.push_back(id);
					} else { // l<=mid<mid+1<=h
						r->lowGreenChild->vSubids.push_back(id);
						r->highGreenChild->vSubids.push_back(id);
					}
					break;
				}
			}
		}
		for (auto &&id: r->rSubids) {
			for (auto &&c: subList[id].constraints) {
				if (c.att == attrId) {
					if (c.highValue <= r->mid) {
						r->leftBlueChild->rSubids.push_back(id);
						if (r->leftBlueChild->mid == c.highValue) r->leftBlueChild->lMidv.push_back(id);
						else if (r->leftBlueChild->mid + 1 == c.lowValue) r->leftBlueChild->rMidv.push_back(id);
					} else if (c.lowValue > r->mid) {
						r->rightBlueChild->rSubids.push_back(id);
						if (r->rightBlueChild->mid == c.highValue) r->rightBlueChild->lMidv.push_back(id);
						else if (r->rightBlueChild->mid + 1 == c.lowValue) r->rightBlueChild->rMidv.push_back(id);
					} else { // l<=mid<mid+1<=h
						r->lowGreenChild->rSubids.push_back(id);
						if (r->lowGreenChild->mid + 1 == c.lowValue)
							r->lowGreenChild->midv.push_back(id);
						r->highGreenChild->rSubids.push_back(id);
						if (r->highGreenChild->mid - 1 == c.highValue)
							r->highGreenChild->midv.push_back(id);
					}
					break;
				}
			}
		}
	}
}

void BGTree_d_vrs::insertVAttrIntoGreenNode(lgreennode_d_vrs *&r, const int &subID, const int &l, const int &attrId,
											const vector<IntervalSub> &subList) {
	r->vSubids.push_back(subID);

	if (r->leftChild != nullptr) {  // 有左子必有右子节点
		if (l <= r->mid)
			insertVAttrIntoGreenNode(r->leftChild, subID, l, attrId, subList);
		else insertVAttrIntoGreenNode(r->rightChild, subID, l, attrId, subList);
	} else if (r->vSubids.size() > maxNodeSize) {
		r->leftChild = new lgreennode_d_vrs(r->l, r->mid, ++numNode, r->levelid + 1, nullptr, nullptr, nullptr);
		r->rightChild = new lgreennode_d_vrs(r->mid + 1, r->h, ++numNode, r->levelid + 1, nullptr, nullptr, nullptr);
		for (auto &&id: r->vSubids) {
			for (auto &&c: subList[id].constraints) {
				if (c.att == attrId) {
					if (c.lowValue <= r->mid) {
						r->leftChild->vSubids.push_back(id);
					} else {
						r->rightChild->vSubids.push_back(id);
					}
					break;
				}
			}
		}
		for (auto &&id: r->rSubids) {
			for (auto &&c: subList[id].constraints) {
				if (c.att == attrId) {
					if (c.lowValue <= r->mid) {
						r->leftChild->rSubids.push_back(id);
						if (r->leftChild->mid + 1 == c.lowValue)
							r->leftChild->midv.push_back(id);
					} else {
						r->rightChild->rSubids.push_back(id);
						if (r->rightChild->mid + 1 == c.lowValue)
							r->rightChild->midv.push_back(id);
					}
					break;
				}
			}
		}
	}
}
void BGTree_d_vrs::insertRAttrIntoGreenNode(lgreennode_d_vrs *&r, const int &subID, const int &l, const int &attrId,
											const vector<IntervalSub> &subList) {
	r->rSubids.push_back(subID);
	if (r->rSubids.size() > BoundaryNumSub) {
		if (r->realBstPtr == nullptr) {
			r->realBstPtr = new bitset<subs>;
			for (auto &&id: r->rSubids)
				(*r->realBstPtr)[id] = 1;
		}
		(*r->realBstPtr)[subID] = 1;
	}
	if (r->mid + 1 == l)
		r->midv.push_back(subID);

	if (r->leftChild != nullptr) {  // 有左子必有右子节点
		if (l <= r->mid)
			insertVAttrIntoGreenNode(r->leftChild, subID, l, attrId, subList);
		else insertVAttrIntoGreenNode(r->rightChild, subID, l, attrId, subList);
	} else if (r->rSubids.size() > maxNodeSize) {
		r->leftChild = new lgreennode_d_vrs(r->l, r->mid, ++numNode, r->levelid + 1, nullptr, nullptr, nullptr);
		r->rightChild = new lgreennode_d_vrs(r->mid + 1, r->h, ++numNode, r->levelid + 1, nullptr, nullptr, nullptr);
		for (auto &&id: r->vSubids) {
			for (auto &&c: subList[id].constraints) {
				if (c.att == attrId) {
					if (c.lowValue <= r->mid) {
						r->leftChild->vSubids.push_back(id);
					} else {
						r->rightChild->vSubids.push_back(id);
					}
					break;
				}
			}
		}
		for (auto &&id: r->rSubids) {
			for (auto &&c: subList[id].constraints) {
				if (c.att == attrId) {
					if (c.lowValue <= r->mid) {
						r->leftChild->rSubids.push_back(id);
						if (r->leftChild->mid + 1 == c.lowValue)
							r->leftChild->midv.push_back(id);
					} else {
						r->rightChild->rSubids.push_back(id);
						if (r->rightChild->mid + 1 == c.lowValue)
							r->rightChild->midv.push_back(id);
					}
					break;
				}
			}
		}
	}
}
void BGTree_d_vrs::insertVAttrIntoGreenNode(hgreennode_d_vrs *&r, const int &subID, const int &h, const int &attrId,
											const vector<IntervalSub> &subList) {
	r->vSubids.push_back(subID);

	if (r->leftChild != nullptr) {  // 有左子必有右子节点
		if (h < r->mid)
			insertVAttrIntoGreenNode(r->leftChild, subID, h, attrId, subList);
		else insertVAttrIntoGreenNode(r->rightChild, subID, h, attrId, subList);
	} else if (r->vSubids.size() > maxNodeSize) {
		r->leftChild = new hgreennode_d_vrs(r->l, r->mid - 1, ++numNode, r->levelid + 1, nullptr, nullptr, nullptr);
		r->rightChild = new hgreennode_d_vrs(r->mid, r->h, ++numNode, r->levelid + 1, nullptr, nullptr, nullptr);
		for (auto &&id: r->vSubids) {
			for (auto &&c: subList[id].constraints) {
				if (c.att == attrId) {
					if (c.highValue < r->mid) {
						r->leftChild->vSubids.push_back(id);
					} else {
						r->rightChild->vSubids.push_back(id);
					}
					break;
				}
			}
		}
	}
}
void BGTree_d_vrs::insertRAttrIntoGreenNode(hgreennode_d_vrs *&r, const int &subID, const int &h, const int &attrId,
											const vector<IntervalSub> &subList) {
	r->rSubids.push_back(subID);
	if (r->rSubids.size() > BoundaryNumSub) {
		if (r->realBstPtr == nullptr) {
			r->realBstPtr = new bitset<subs>;
			for (auto &&id: r->rSubids)
				(*r->realBstPtr)[id] = 1;
		}
		(*r->realBstPtr)[subID] = 1;
	}
	if (r->mid - 1 == h)
		r->midv.push_back(subID);

	if (r->leftChild != nullptr) {  // 有左子必有右子节点
		if (h < r->mid)
			insertVAttrIntoGreenNode(r->leftChild, subID, h, attrId, subList);
		else insertVAttrIntoGreenNode(r->rightChild, subID, h, attrId, subList);
	} else if (r->rSubids.size() > maxNodeSize) {
		r->leftChild = new hgreennode_d_vrs(r->l, r->mid - 1, ++numNode, r->levelid + 1, nullptr, nullptr, nullptr);
		r->rightChild = new hgreennode_d_vrs(r->mid, r->h, ++numNode, r->levelid + 1, nullptr, nullptr, nullptr);
		for (auto &&id: r->rSubids) {
			for (auto &&c: subList[id].constraints) {
				if (c.att == attrId) {
					if (c.highValue < r->mid) {
						r->leftChild->rSubids.push_back(id);
						if (r->leftChild->mid - 1 == c.highValue)
							r->leftChild->midv.push_back(id);
					} else {
						r->rightChild->rSubids.push_back(id);
						if (r->rightChild->mid - 1 == c.highValue)
							r->rightChild->midv.push_back(id);
					}
					break;
				}
			}
		}
	}
}
bool BGTree_d_vrs::deleteSubscription(IntervalSub sub) {
	bool find = true;
	for (auto &&c: sub.constraints) {
		/*find &= deleteFromBlueNode(roots[c.att], sub.id, c.lowValue,
								   c.highValue);*/
	}
	numSub--;
	return find;
}
//
//bool BGTree_d_vrs::deleteFromBlueNode(bluenode_d_vrs *&r, const int &subID, const int &l, const int &h) {
//	bool find = false;
//	if (r->realBstPtr != nullptr) {
//		(*(r->realBstPtr))[subID] = 0;
//		if (r->subids.size() < BoundaryNumSub)
//			delete r->realBstPtr;
//	}
//	for (vector<int>::const_iterator it = r->subids.cbegin(); it != r->subids.cend(); it++) {
//		if (*it == subID) {
//			r->subids.erase(it);
//			find = true;
//			break;
//		}
//	}
//	if (r->mid == h) {
//		find = false;
//		for (vector<int>::const_iterator it = r->lMidv.cbegin(); it != r->lMidv.cend(); it++) {
//			if (*it == subID) {
//				r->lMidv.erase(it);
//				find = true;
//				break;
//			}
//		}
//	} else if (r->mid + 1 == l) {
//		find = false;
//		for (vector<int>::const_iterator it = r->rMidv.cbegin(); it != r->rMidv.cend(); it++) {
//			if (*it == subID) {
//				r->rMidv.erase(it);
//				find = true;
//				break;
//			}
//		}
//	}
//
//	if (r->leftBlueChild != nullptr) { // has child
//		if (h <= r->mid) {
//			find &= deleteFromBlueNode(r->leftBlueChild, subID, l, h);
//		} else if (l > r->mid) {
//			find &= deleteFromBlueNode(r->rightBlueChild, subID, l, h);
//		} else {
//			find &= deleteFromGreenNode(r->lowGreenChild, subID, l);
//			find &= deleteFromGreenNode(r->highGreenChild, subID, h);
//		}
//
//		if (r->subids.size() < maxNodeSize) { // has child and too small node
//			delete r->leftBlueChild, r->rightBlueChild, r->lowGreenChild, r->highGreenChild;
//		}
//	}
//	return find;
//}
//
//bool BGTree_d_vrs::deleteFromGreenNode(lgreennode_d_vrs *&r, const int &subID, const int &l) {
//	bool find = false;
//	if (r->realBstPtr != nullptr) {
//		(*(r->realBstPtr))[subID] = 0;
//		if (r->subids.size() < BoundaryNumSub)
//			delete r->realBstPtr;
//	}
//	for (vector<int>::const_iterator it = r->subids.cbegin(); it != r->subids.cend(); it++) {
//		if (*it == subID) {
//			r->subids.erase(it);
//			find = true;
//			break;
//		}
//	}
//	if (r->mid + 1 == l) {
//		find = false;
//		for (vector<int>::const_iterator it = r->midv.cbegin(); it != r->midv.cend(); it++) {
//			if (*it == subID) {
//				r->midv.erase(it);
//				find = true;
//				break;
//			}
//		}
//	}
//	if (r->leftChild != nullptr) { // has child
//		if (l <= r->mid) {
//			find &= deleteFromGreenNode(r->leftChild, subID, l);
//		} else {
//			find &= deleteFromGreenNode(r->rightChild, subID, l);
//		}
//		if (r->subids.size() < maxNodeSize) { // has child and too small node
//			delete r->leftChild, r->rightChild, r->realBstPtr;
//		}
//	}
//	return find;
//}
//
//bool BGTree_d_vrs::deleteFromGreenNode(hgreennode_d_vrs *&r, const int &subID, const int &h) {
//	bool find = false;
//	if (r->realBstPtr != nullptr) {
//		(*(r->realBstPtr))[subID] = 0;
//		if (r->subids.size() < BoundaryNumSub)
//			delete r->realBstPtr;
//	}
//	for (vector<int>::const_iterator it = r->subids.cbegin(); it != r->subids.cend(); it++) {
//		if (*it == subID) {
//			r->subids.erase(it);
//			find = true;
//			break;
//		}
//	}
//	if (r->mid - 1 == h) {
//		find = false;
//		for (vector<int>::const_iterator it = r->midv.cbegin(); it != r->midv.cend(); it++) {
//			if (*it == subID) {
//				r->midv.erase(it);
//				find = true;
//				break;
//			}
//		}
//	}
//	if (r->leftChild != nullptr) { // has child
//		if (h < r->mid) {
//			find &= deleteFromGreenNode(r->leftChild, subID, h);
//		} else {
//			find &= deleteFromGreenNode(r->rightChild, subID, h);
//		}
//
//		if (r->subids.size() < maxNodeSize) { // has child and too small node
//			delete r->leftChild, r->rightChild;
//		}
//	}
//	return find;
//}
//
//void BGTree_d_vrs::match_forward_native(const Pub &pub, int &matchSubs, const vector<IntervalSub> &subList) {
//	memcpy(counter, subPredicate, sizeof(subPredicate));
//	for (auto &&pi: pub.pairs)
//		match_forward_blueNode(roots[pi.att], pi.att, pi.value, subList);
//	for (int i = 0; i < subs; i++)
//		if (counter[i] == 0) {
//#ifdef DEBUG
//			numEffectivePredicate += subPredicate[i];
//#endif
//			++matchSubs;
//			//cout << "BG-Tree matches sub: " << i << endl;
//		}
//}
//
//void
//BGTree_d_vrs::match_forward_blueNode(bluenode_d_vrs *&r, const int &att, const int &value,
//									 const vector<IntervalSub> &subList) {
//	if (r->leftBlueChild == nullptr) { // 1. 叶子节点暴力处理
//#ifdef DEBUG
//		numProcessTwoCmpNode++;
//		numProcessTwoCmpPredicate += r->subids.size();
//#endif
//		for (auto &&i: r->subids) {
//			for (auto &&pi: subList[i].constraints)
//				if (att == pi.att) {
//					if (pi.lowValue <= value && value <= pi.highValue)
//						counter[i]--;
//					break;
//				}
//		}
//	} else if (value < r->mid) { // 2. 小于左中点, 检索两个子节点
//		match_forward_blueNode(r->leftBlueChild, att, value, subList);
//		match_forward_lgreenNode(r->lowGreenChild, att, value, subList);
//	} else if (value == r->mid) { // 3. 等于左中点, 直接得到匹配结果
//#ifdef DEBUG
//		hit++;
//		numProcessExactNode++;
//		numProcessExactPredicate += r->lMidv.size() + r->lowGreenChild->subids.size();
//#endif
//		for (auto &&id: r->lMidv) {
//			counter[id]--;
//		}
//		for (auto &&id: r->lowGreenChild->subids) {
//			counter[id]--;
//		}
//	} else if (value == r->mid + 1) { // 4. 等于右中点, 直接得到匹配结果
//#ifdef DEBUG
//		hit++;
//		numProcessExactNode++;
//		numProcessExactPredicate += r->rMidv.size() + r->highGreenChild->subids.size();
//#endif
//		for (auto &&id: r->rMidv) {
//			counter[id]--;
//		}
//		for (auto &&id: r->highGreenChild->subids) {
//			counter[id]--;
//		}
//	} else { // 5. value > r->mid + 1 大于右中节点, 检索两个子节点
//		match_forward_blueNode(r->rightBlueChild, att, value, subList);
//		forward_match_hgreenNode(r->highGreenChild, att, value, subList);
//	}
//}
//
//void BGTree_d_vrs::match_forward_lgreenNode(lgreennode_d_vrs *&r, const int &att, const int &value,
//											const vector<IntervalSub> &subList) {
//	// 把最有可能出现的放前面
//	if (r->leftChild == nullptr) { // 1. 叶子节点暴力处理
//#ifdef DEBUG
//		numProcessOneCmpNode++;
//		numProcessOneCmpPredicate += r->subids.size();
//#endif
//		for (auto &&id: r->subids) {
//			for (auto &&pi: subList[id].constraints)
//				if (att == pi.att) {
//					if (pi.lowValue <= value)
//						counter[id]--;
//					break;
//				}
//		}
//	} else if (value < r->mid) { // 2. 小于左中点, 检索左子节点
//		match_forward_lgreenNode(r->leftChild, att, value, subList);
//	} else if (value > r->mid + 1) { // 5. 大于右中点, 左子节点完全匹配, 检索右子节点
//#ifdef DEBUG
//		numProcessExactNode++;
//		numProcessExactPredicate += r->leftChild->subids.size();
//#endif
//		for (auto &&id: r->leftChild->subids) {
//			counter[id]--;
//		}
//		match_forward_lgreenNode(r->rightChild, att, value, subList);
//	} else if (value == r->mid) { // 3. 等于左中点, 左子节点全部匹配
//#ifdef DEBUG
//		numProcessExactNode++;
//		numProcessExactPredicate += r->leftChild->subids.size();
//#endif
//		for (auto &&id: r->leftChild->subids) {
//			counter[id]--;
//		}
//	} else { // 4. if (value == r->mid + 1), 等于 r->mid+1, 直接得到匹配结果
//#ifdef DEBUG
//		numProcessExactNode++;
//		numProcessExactPredicate += r->midv.size() + r->leftChild->subids.size();
//#endif
//		for (auto &&id: r->midv) {
//			counter[id]--;
//		}
//		for (auto &&id: r->leftChild->subids) {
//			counter[id]--;
//		}
//	}
//}
//
//void BGTree_d_vrs::forward_match_hgreenNode(hgreennode_d_vrs *&r, const int &att, const int &value,
//											const vector<IntervalSub> &subList) {
//	// 把最有可能出现的放前面
//	if (r->leftChild == nullptr) { // 1. 叶子节点暴力处理
//#ifdef DEBUG
//		numProcessOneCmpNode++;
//		numProcessOneCmpPredicate += r->subids.size();
//#endif
//		for (auto &&id: r->subids) {
//			for (auto &&pi: subList[id].constraints)
//				if (att == pi.att) {
//					if (value <= pi.highValue)
//						counter[id]--;
//					break;
//				}
//		}
//	} else if (value > r->mid) { // 2. 大于右中点, 检索右子节点
//		forward_match_hgreenNode(r->rightChild, att, value, subList);
//	} else if (value < r->mid - 1) { // 5. 小于左中点, 右子节点完全匹配, 检索左子节点
//#ifdef DEBUG
//		numProcessExactNode++;
//		numProcessExactPredicate += r->rightChild->subids.size();
//#endif
//		for (auto &&id: r->rightChild->subids) {
//			counter[id]--;
//		}
//		forward_match_hgreenNode(r->leftChild, att, value, subList);
//	} else if (value == r->mid) { // 3. 等于右中点, 右子节点全部匹配
//#ifdef DEBUG
//		numProcessExactNode++;
//		numProcessExactPredicate += r->rightChild->subids.size();
//#endif
//		for (auto &&id: r->rightChild->subids) {
//			counter[id]--;
//		}
//	} else { // 4. if (value == r->mid - 1), 等于左中点, 直接得到匹配结果
//#ifdef DEBUG
//		numProcessExactNode++;
//		numProcessExactPredicate += r->midv.size() + r->rightChild->subids.size();
//#endif
//		for (auto &&id: r->midv) {
//			counter[id]--;
//		}
//		for (auto &&id: r->rightChild->subids) {
//			counter[id]--;
//		}
//	}
//}
//
//void BGTree_d_vrs::match_backward_native(const Pub &pub, int &matchSubs, const vector<IntervalSub> &subList) {
//	bitset<subs> gB; // global bitset
//	vector<bool> attExist(atts, false);
//	for (auto &&pi: pub.pairs) {
//		attExist[pi.att] = true;
//		match_backward_blueNode(roots[pi.att], pi.att, pi.value, subList, gB);
//	}
//	_for(i, 0, atts) if (!attExist[i])
//			gB = gB | nnB[i];
//	matchSubs = numSub - gB.count();
//#ifdef DEBUG
//	for (int i = 0; i < subs; i++)
//		if (gB[i] == 0)
//			numEffectivePredicate += subPredicate[i];
//#endif
//}
//
//void BGTree_d_vrs::match_backward_blueNode(bluenode_d_vrs *&r, const int &att, const int &value,
//												  const vector<IntervalSub> &subList, bitset<subs> &gB) {
//	if (r->leftBlueChild == nullptr) {
//#ifdef DEBUG
//		numProcessTwoCmpNode++;
//		numProcessTwoCmpPredicate += r->subids.size();
//#endif
//		for (auto &&i: r->subids) {
//			for (auto &&pi: subList[i].constraints)
//				if (att == pi.att) {
//					if (value < pi.lowValue || pi.highValue < value)
//						gB[i] = 1;
//					break;
//				}
//		}
//	} else if (value <= r->mid) {
//		if (r->rightBlueChild->realBstPtr == nullptr) {
//#ifdef DEBUG
//			numProcessExactNode++;
//			numProcessExactPredicate += r->rightBlueChild->subids.size();
//#endif
//			for (auto &&id: r->rightBlueChild->subids)
//				gB[id] = 1;
//		} else {
//#ifdef DEBUG
//			numProcessExactNode++;
//			numProcessExactPredicate += r->rightBlueChild->subids.size();
//			numBitsetOperation++;
//#endif
//			gB = gB | *r->rightBlueChild->realBstPtr;
//		}
//		if (value == r->mid && r->leftBlueChild->realBstPtr != nullptr) {
//#ifdef DEBUG
//			numProcessExactNode++;
//			numProcessExactPredicate += r->leftBlueChild->subids.size() - r->lMidv.size();
//			numBitsetOperation++;
//			hit++;
//#endif
//			bitset<subs> bst_c = *r->leftBlueChild->realBstPtr;
//			for (auto &&id: r->lMidv)
//				bst_c[id] = 0;
//			gB = gB | bst_c;
//		} else {
//			match_backward_blueNode(r->leftBlueChild, att, value, subList, gB);
//			match_backward_lgreenNode(r->lowGreenChild, att, value, subList, gB);
//		}
//	} else {
//		if (r->leftBlueChild->realBstPtr == nullptr) {
//#ifdef DEBUG
//			numProcessExactNode++;
//			numProcessExactPredicate += r->leftBlueChild->subids.size();
//#endif
//			for (auto &&id: r->leftBlueChild->subids)
//				gB[id] = 1;
//		} else {
//#ifdef DEBUG
//			numProcessExactNode++;
//			numProcessExactPredicate += r->leftBlueChild->subids.size();
//			numBitsetOperation++;
//#endif
//			gB = gB | *r->leftBlueChild->realBstPtr;
//		}
//		if (value == r->mid + 1 && r->rightBlueChild->realBstPtr != nullptr) {
//#ifdef DEBUG
//			numProcessExactNode++;
//			numProcessExactPredicate += r->rightBlueChild->subids.size() - r->rMidv.size();
//			numBitsetOperation++;
//			hit++;
//#endif
//			bitset<subs> bst_c = *r->rightBlueChild->realBstPtr;
//			for (auto &&id: r->rMidv)
//				bst_c[id] = 0;
//			gB = gB | bst_c;
//		} else {
//			match_backward_blueNode(r->rightBlueChild, att, value, subList, gB);
//			backward_match_hgreenNode_native(r->highGreenChild, att, value, subList, gB);
//		}
//	}
//}
//
//void
//BGTree_d_vrs::match_backward_lgreenNode(lgreennode_d_vrs *&l, const int &att, const int &value,
//											   const vector<IntervalSub> &subList, bitset<subs> &gB) {
//	// 把最有可能出现的放前面
//	if (l->leftChild == nullptr) { // 1. 叶子节点暴力处理
//#ifdef DEBUG
//		numProcessOneCmpNode++;
//		numProcessOneCmpPredicate += l->subids.size();
//#endif
//		for (auto &&id: l->subids) {
//			for (auto &&pi: subList[id].constraints)
//				if (att == pi.att) {
//					if (pi.lowValue > value)
//						gB[id] = 1;
//					break;
//				}
//		}
//	} else if (value <= l->mid) {
//		if (l->rightChild->realBstPtr == nullptr) {
//#ifdef DEBUG
//			numProcessExactNode++;
//			numProcessExactPredicate += l->rightChild->subids.size();
//#endif
//			for (auto &&id: l->rightChild->subids)
//				gB[id] = 1;
//		} else {
//#ifdef DEBUG
//			numProcessExactNode++;
//			numProcessExactPredicate += l->rightChild->subids.size();
//			numBitsetOperation++;
//#endif
//			gB = gB | *l->rightChild->realBstPtr;
//		}
//		if (value < l->mid)
//			match_backward_lgreenNode(l->leftChild, att, value, subList, gB);
//#ifdef DEBUG
//		if (value == l->mid)
//			hit++;
//#endif
//	} else { // l->mid < value
//		if (l->mid + 1 == value && l->rightChild->realBstPtr != nullptr) {
//#ifdef DEBUG
//			numProcessExactNode++;
//			numProcessExactPredicate += l->rightChild->subids.size() - l->midv.size();
//			numBitsetOperation++;
//			hit++;
//#endif
//			bitset<subs> bst_c = *l->rightChild->realBstPtr;
//			for (auto &&id: l->midv)
//				bst_c[id] = 0;
//			gB = gB | bst_c;
//		} else {
//			match_backward_lgreenNode(l->rightChild, att, value, subList, gB);
//		}
//	}
//}
//
//void
//BGTree_d_vrs::backward_match_hgreenNode_native(hgreennode_d_vrs *&r, const int &att, const int &value,
//											   const vector<IntervalSub> &subList, bitset<subs> &gB) {
//	if (r->leftChild == nullptr) { // 1. 叶子节点暴力处理
//#ifdef DEBUG
//		numProcessOneCmpNode++;
//		numProcessOneCmpPredicate += r->subids.size();
//#endif
//		for (auto &&id: r->subids) {
//			for (auto &&pi: subList[id].constraints)
//				if (att == pi.att) {
//					if (pi.highValue < value)
//						gB[id] = 1;
//					break;
//				}
//		}
//	} else if (value >= r->mid) { // 2. fall into right subInterval
//		if (r->leftChild->realBstPtr == nullptr) { // leftChild must be all-unmatch
//#ifdef DEBUG
//			numProcessExactNode++;
//			numProcessExactPredicate += r->leftChild->subids.size();
//#endif
//			for (auto &&id: r->leftChild->subids)
//				gB[id] = 1;
//		} else {
//#ifdef DEBUG
//			numProcessExactNode++;
//			numProcessExactPredicate += r->leftChild->subids.size();
//			numBitsetOperation++;
//#endif
//			gB = gB | *r->leftChild->realBstPtr;
//		}
//		if (value > r->mid)
//			backward_match_hgreenNode_native(r->rightChild, att, value, subList, gB);
//#ifdef DEBUG
//		if (value == r->mid)
//			hit++;
//#endif
//	} else { // value < r->mid
//		if (r->mid == value + 1 && r->leftChild->realBstPtr != nullptr) {
//#ifdef DEBUG
//			numProcessExactNode++;
//			numProcessExactPredicate += r->leftChild->subids.size() - r->midv.size();
//			numBitsetOperation++;
//			hit++;
//#endif
//			bitset<subs> bst_c = *r->leftChild->realBstPtr;
//			for (auto &&id: r->midv)
//				bst_c[id] = 0;
//			gB = gB | bst_c;
//		} else {
//			backward_match_hgreenNode_native(r->leftChild, att, value, subList, gB);
//		}
//	}
//}
//
//int BGTree_d_vrs::calMemory() {
//	double size = 0.0; // Byte
//	size += sizeof(roots) + sizeof(bluenode_d_vrs *) * atts;
//	bluenode_d_vrs *b;
//	lgreennode_d_vrs *l;
//	hgreennode_d_vrs *h;
//	for (int i = 0; i < atts; i++) {
//		queue<bluenode_d_vrs *> bq;
//		queue<lgreennode_d_vrs *> lq;
//		queue<hgreennode_d_vrs *> hq;
//		bq.push(roots[i]);
//		while (!bq.empty() || !lq.empty() || !hq.empty()) {
//			if (!bq.empty()) {
//				b = bq.front();
//				bq.pop();
//				size += sizeof(b) + (5 + b->subids.size() + b->lMidv.size() + b->rMidv.size()) * sizeof(int) +
//						sizeof(vector<int>) * 3;
//				size +=
//					sizeof(lgreennode_d_vrs *) + sizeof(hgreennode_d_vrs *) + sizeof(bluenode_d_vrs *) * 2 +
//					sizeof(bitset<subs> *);
//				if (b->leftBlueChild) {
//					bq.push(b->leftBlueChild);
//					bq.push(b->rightBlueChild);
//					lq.push(b->lowGreenChild);
//					hq.push(b->highGreenChild);
//				}
//				if (b->realBstPtr) size += sizeof(bitset<subs>);
//			}
//			if (!lq.empty()) {
//				l = lq.front();
//				lq.pop();
//				size += sizeof(l) + (5 + l->subids.size() + l->midv.size()) * sizeof(int) + sizeof(vector<int>) * 2 +
//						sizeof(bitset<subs> *);
//				if (l->leftChild != nullptr) {
//					size += sizeof(lgreennode_d_vrs *) * 2;
//					lq.emplace(l->leftChild);
//					lq.emplace(l->rightChild);
//				}
//				if (l->realBstPtr)size += sizeof(bitset<subs>);
//			}
//			if (!hq.empty()) {
//				h = hq.front();
//				hq.pop();
//				size += sizeof(h) + (5 + h->subids.size() + h->midv.size()) * sizeof(int) + sizeof(vector<int>) * 2 +
//						sizeof(bitset<subs> *);
//				if (h->leftChild != nullptr) {
//					size += sizeof(hgreennode_d_vrs *) * 2;
//					hq.emplace(h->leftChild);
//					hq.emplace(h->rightChild);
//				}
//				if (h->realBstPtr)size += sizeof(bitset<subs>);
//			}
//		}
//	}
//	size += sizeof(int) * subs * 2; // subPredicate counter
//	size += sizeof(nB) + sizeof(nB[0]) * atts; // nB or nnB
//	size = size / 1024 / 1024; // MB
//	return (int) size;
//}
//
//void BGTree_d_vrs::printBGTree() {
//	vector<vector<vector<tuple<int, int, int, int>>>> nodeInfo(atts, vector<vector<tuple<int, int, int, int>>>());
//	int height = 0;
//	_for(i, 0, atts) {
//		height = 0;
//		queue<bluenode_d_vrs *> bNextLevel;
//		queue<lgreennode_d_vrs *> lgNextLevel;
//		queue<hgreennode_d_vrs *> hgNextLevel;
//		bNextLevel.push(roots[i]);
//		while (!bNextLevel.empty() || !lgNextLevel.empty() || !hgNextLevel.empty()) { // for each level
//			nodeInfo[i].push_back(vector<tuple<int, int, int, int>>());
//			int b = bNextLevel.size(), l = lgNextLevel.size(), r = hgNextLevel.size();
//			_for(j, 0, b) {
//				bluenode_d_vrs *t = bNextLevel.front();
//				bNextLevel.pop();
//				nodeInfo[i][height].push_back(make_tuple(1, t->l, t->h, t->subids.size()));
//				if (t->leftBlueChild) bNextLevel.push(t->leftBlueChild);
//				if (t->rightBlueChild) bNextLevel.push(t->rightBlueChild);
//				if (t->lowGreenChild) lgNextLevel.push(t->lowGreenChild);
//				if (t->highGreenChild) hgNextLevel.push(t->highGreenChild);
//			}
//			_for(j, 0, l) {
//				lgreennode_d_vrs *t = lgNextLevel.front();
//				lgNextLevel.pop();
//				nodeInfo[i][height].push_back(make_tuple(2, t->l, t->h, t->subids.size()));
//				if (t->leftChild) lgNextLevel.push(t->leftChild);
//				if (t->rightChild) lgNextLevel.push(t->rightChild);
//			}
//			_for(j, 0, r) {
//				hgreennode_d_vrs *t = hgNextLevel.front();
//				hgNextLevel.pop();
//				nodeInfo[i][height].push_back(make_tuple(3, t->l, t->h, t->subids.size()));
//				if (t->leftChild) hgNextLevel.push(t->leftChild);
//				if (t->rightChild) hgNextLevel.push(t->rightChild);
//			}
//			height++;
//		}
//	}
//	_for(i, 0, 1) {
//		_for(j, 0, nodeInfo[i].size()) {
//			cout << "a" << i << ", h" << j << ", nodeNum= " << nodeInfo[i][j].size() << "\n";
//			int nodeId = -1, lv = -1, hv = -1, maxNumNodeSub = -1;
//			sort(nodeInfo[i][j].begin(), nodeInfo[i][j].end(),
//				 [](const tuple<int, int, int, int> &a, const tuple<int, int, int, int> &b) {
//					 return get<0>(a) == get<0>(b) ? get<3>(a) > get<3>(b) : get<0>(a) < get<0>(b);
//				 });
//			_for(k, 0, nodeInfo[i][j].size()) {
//				cout << "(" << get<0>(nodeInfo[i][j][k]) << "," << get<1>(nodeInfo[i][j][k]) << ","
//					 << get<2>(nodeInfo[i][j][k]) << ", " << get<3>(nodeInfo[i][j][k]) << "), ";
//				if (k < nodeInfo[i][j].size() - 1 && (get<0>(nodeInfo[i][j][k]) != get<0>(nodeInfo[i][j][k + 1])))
//					cout << "\n";
//				if (maxNumNodeSub < get<3>(nodeInfo[i][j][k])) {
//					nodeId = get<0>(nodeInfo[i][j][k]);
//					lv = get<1>(nodeInfo[i][j][k]);
//					hv = get<2>(nodeInfo[i][j][k]);
//					maxNumNodeSub = get<3>(nodeInfo[i][j][k]);
//				}
//			}
//			cout << "\nNodeID= " << nodeId << " [" << lv << ", " << hv << "], size= " << maxNumNodeSub << "\n\n";
//		}
//		cout << "\n";
//	}
//	cout << "ExactNode: " << numProcessExactNode << ", oneCmpNode: " << numProcessOneCmpNode << ", twoCmpNode: "
//		 << numProcessTwoCmpNode \
// << "\nExactPredicate: " << numProcessExactPredicate << ", oneCmpPredicate: " << numProcessOneCmpPredicate
//		 << ", twoCmpPredicate: " << numProcessTwoCmpPredicate \
// << "\nEffectivePredicate: " << numEffectivePredicate << ", effectiveRate: "
//		 << (double) numEffectivePredicate /
//			(double) (numProcessExactPredicate + numProcessOneCmpPredicate + numProcessTwoCmpPredicate + 1)
//		 << "\nOR: " << numBitsetOperation << ", AvgHit: " << hit << ".\n\n";
//
//}
//
//int BGTree_d_vrs::getHeight() {
//	int height = 0;
//	queue<bluenode_d_vrs *> bq;
//	queue<lgreennode_d_vrs *> lq;
//	queue<hgreennode_d_vrs *> hq;
//	bluenode_d_vrs *b;
//	lgreennode_d_vrs *l;
//	hgreennode_d_vrs *h;
//	_for(i, 0, atts) bq.emplace(roots[i]);
//	while (!bq.empty() || !lq.empty() || !hq.empty()) {
//		if (!bq.empty()) {
//			b = bq.front();
//			bq.pop();
//			height = max(height, b->levelid);
//			if (b->leftBlueChild != nullptr) {
//				bq.emplace(b->leftBlueChild);
//				bq.emplace(b->rightBlueChild);
//				lq.emplace(b->lowGreenChild);
//				hq.emplace(b->highGreenChild);
//			}
//		}
//		if (!lq.empty()) {
//			l = lq.front();
//			lq.pop();
//			height = max(height, l->levelid);
//			if (l->leftChild != nullptr) {
//				lq.emplace(l->leftChild);
//				lq.emplace(l->rightChild);
//			}
//		}
//		if (!hq.empty()) {
//			h = hq.front();
//			hq.pop();
//			height = max(height, h->levelid);
//			if (h->leftChild != nullptr) {
//				hq.emplace(h->leftChild);
//				hq.emplace(h->rightChild);
//			}
//		}
//	}
//	return height;
//}