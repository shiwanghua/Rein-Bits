#include "HEM3_ASO.h"

HEM3_ASO::HEM3_ASO(int type)
{
    numSub = 0;
    numDimension = atts;
    buckStep = (valDom - 1) / buks + 1;
    numBucket = (valDom - 1) / buckStep + 1;

    numAttrGroup = attrGroup;
    attrGroupSize = (numDimension + numAttrGroup - 1) / numAttrGroup; // 最后一组可能不满
    attrGroupBits.resize(numAttrGroup);

    (be == -1) ? numBits = be2 : numBits = pow(2, be) + 1; // 每个维度上 bits 数组个数

    data[0].resize(numDimension, vector<vector<Combo>>(numBucket));
    data[1].resize(numDimension, vector<vector<Combo>>(numBucket));

    bits.resize(numDimension, vector<bitset<subs>>(numBits));

    endBucket[0] = new int *[numDimension];
    endBucket[1] = new int *[numDimension];
    bitsID = new int *[numDimension];
    _for(i, 0, numDimension)
    {
        endBucket[0][i] = new int[numBucket];
        endBucket[1][i] = new int[numBucket];
        bitsID[i] = new int[numBucket];
    }

    fix[0].resize(numDimension, vector<int>(numBucket + 1));
    fix[1].resize(numDimension, vector<int>(numBucket + 1));
    string TYPE;
    if (type == HEM3_D_VASO)
        TYPE = "HEM3_D_VASO)";
    else if (type == HEM3_D_RASO)
        TYPE = "HEM3_D_RASO";
    else if (type == HEM3_D_RASO_AVXOR_PARALLEL)
    {
        TYPE = "HEM3_D_RASO_AVXOR" + to_string(BlockSize) + "_PARALLEL";
        threadPool.initThreadPool(parallelDegree);
    }
    cout << "ExpID = " << expID << ". " + TYPE + ": bitset number = " << numBits << ", bucketStep = " << buckStep
         << ", numBucket = " << numBucket << ", attrGroupNum = " << numAttrGroup << ", attGroupSize = " << attrGroupSize
         << endl;
}

HEM3_ASO::~HEM3_ASO()
{
    _for(i, 0, numDimension)
    {
        delete[] endBucket[0][i], endBucket[1][i], bitsID[i];
    }
    delete[] endBucket[0], endBucket[1], bitsID;
}

void HEM3_ASO::insert_VASO(const IntervalSub& sub)
{
    Combo c;
    c.subID = sub.id;
    for (auto &&iCnt : sub.constraints)
    {
        attrGroupBits[iCnt.att / attrGroupSize][sub.id] = 1;
        c.val = iCnt.lowValue;
        data[0][iCnt.att][iCnt.lowValue / buckStep].emplace_back(c);
        c.val = iCnt.highValue;
        data[1][iCnt.att][iCnt.highValue / buckStep].emplace_back(c);
    }
    numSub++;
}

void HEM3_ASO::insert_RASO(const IntervalSub &sub)
{
    for (int attGroupNo = sub.constraints[0].att / attrGroupSize, gi = 0; gi < numAttrGroup; gi++)
    {
        if (gi != attGroupNo)
            attrGroupBits[gi][sub.id] = 1;
    }
    Combo c;
    c.subID = sub.id;
    for (auto &&iCnt : sub.constraints)
    {
        c.val = iCnt.lowValue;
        data[0][iCnt.att][iCnt.lowValue / buckStep].emplace_back(c);
        c.val = iCnt.highValue;
        data[1][iCnt.att][iCnt.highValue / buckStep].emplace_back(c);
    }
    numSub++;
}

// 在线插入，已经构建好索引结构了，插入谓词、标记订阅
void HEM3_ASO::insert_online_VASO(const IntervalSub &sub)
{
    int b, bucketID;
    Combo c;
    c.subID = sub.id;

    for (auto &&iCnt : sub.constraints)
    {
        bits[iCnt.att][0][sub.id] = 1; // 默认 0 号位集存储所有订阅，即原来的 fullBits
        attrGroupBits[iCnt.att / attrGroupSize][sub.id] = 1;
        b = bitsID[iCnt.att][bucketID]; // = 这个桶所属的组号（从 LVE看）

        bucketID = iCnt.lowValue / buckStep;
        c.val = iCnt.lowValue;
        data[0][iCnt.att][bucketID].emplace_back(c);
        _for(q, b + 1, numBits) bits[iCnt.att][q][sub.id] = 1;

        bucketID = iCnt.highValue / buckStep;
        c.val = iCnt.highValue;
        data[1][iCnt.att][bucketID].emplace_back(c);
        _for(q, 1, b) bits[iCnt.att][q][sub.id] = 1;
    }
    numSub++;
}

void HEM3_ASO::insert_online_RASO(const IntervalSub &sub)
{
    int b, bucketID;
    Combo c;
    c.subID = sub.id;

    for (int attGroupNo = sub.constraints[0].att / attrGroupSize, i = 0; i < numAttrGroup; i++)
    {
        if (i != attGroupNo)
            attrGroupBits[i][sub.id] = 1;
    }

    for (auto &&iCnt : sub.constraints)
    {
        bits[iCnt.att][0][sub.id] = 1;
        b = bitsID[iCnt.att][bucketID]; // = 这个桶所属的组号（从 LVE看）

        bucketID = iCnt.lowValue / buckStep;
        c.val = iCnt.lowValue;
        data[0][iCnt.att][bucketID].emplace_back(c);
        _for(q, b + 1, numBits) bits[iCnt.att][q][sub.id] = 1;

        bucketID = iCnt.highValue / buckStep;
        c.val = iCnt.highValue;
        data[1][iCnt.att][bucketID].emplace_back(c);
        _for(q, 1, b) bits[iCnt.att][q][sub.id] = 1;
    }
    numSub++;
}

bool HEM3_ASO::deleteSubscription_VASO(const IntervalSub &sub)
{
    int find = 0, b, bucketID, id = sub.id;
    vector<Combo>::const_iterator it;

    for (auto &&iCnt : sub.constraints)
    {
        bits[iCnt.att][0][sub.id] = 0;
        attrGroupBits[iCnt.att / attrGroupSize][sub.id] = 0;
        b = bitsID[iCnt.att][bucketID];

        bucketID = iCnt.lowValue / buckStep;
        for (it = data[0][iCnt.att][bucketID].cbegin(); it != data[0][iCnt.att][bucketID].cend(); it++)
            if (it->subID == id)
            {
                data[0][iCnt.att][bucketID].erase(it);
                find++;
                break;
            }
        _for(q, b + 1, numBits) bits[iCnt.att][q][sub.id] = 0;

        bucketID = iCnt.highValue / buckStep;
        for (it = data[1][iCnt.att][bucketID].cbegin(); it != data[1][iCnt.att][bucketID].cend(); it++)
            if (it->subID == id)
            {
                data[1][iCnt.att][bucketID].erase(it);
                find++;
                break;
            }
        _for(q, 1, b) bits[iCnt.att][q][sub.id] = 0;
    }
    if (find == sub.size << 1)
    {
        numSub--;
        return true;
    }
    return false;
}

bool HEM3_ASO::deleteSubscription_RASO(const IntervalSub &sub)
{
    int find = 0, b, bucketID, id = sub.id;

    // int attGroupNo = sub.constraints[0].att / attrGroupSize;
    _for(i, 0, numAttrGroup)
    {
        // if (i != attGroupNo)
        attrGroupBits[i][sub.id] = 0;
    }

    for (auto &&iCnt : sub.constraints)
    {
        bits[iCnt.att][0][sub.id] = 0;
        b = bitsID[iCnt.att][bucketID];

        bucketID = iCnt.lowValue / buckStep;
        vector<Combo>::const_iterator it;
        for (it = data[0][iCnt.att][bucketID].cbegin(); it != data[0][iCnt.att][bucketID].cend(); it++)
            if (it->subID == id)
            {
                data[0][iCnt.att][bucketID].erase(it);
                find++;
                break;
            }
        _for(q, b + 1, numBits) bits[iCnt.att][q][sub.id] = 0;

        bucketID = iCnt.highValue / buckStep;
        for (it = data[1][iCnt.att][bucketID].cbegin(); it != data[1][iCnt.att][bucketID].cend(); it++)
            if (it->subID == id)
            {
                data[1][iCnt.att][bucketID].erase(it);
                find++;
                break;
            }
        _for(q, 1, b) bits[iCnt.att][q][sub.id] = 0;
    }
    if (find == sub.size << 1)
    {
        numSub--;
        return true;
    }
    return false;
}

void HEM3_ASO::initBits()
{
    // 考虑多次初始化
    _for(i, 0, numDimension) delete[] endBucket[0][i], endBucket[1][i], bitsID[i], bitsID[i];
    delete[] endBucket[0], endBucket[1], bitsID;
    endBucket[0] = new int *[numDimension];
    endBucket[1] = new int *[numDimension];
    bitsID = new int *[numDimension];
    _for(i, 0, numDimension)
    {
        endBucket[0][i] = new int[numBucket];
        endBucket[1][i] = new int[numBucket]; // 初始化时计算一下，插入搜索统一只用 LVE 上的一套
        bitsID[i] = new int[numBucket];
    }
    bits[0].clear(), bits[1].clear();
    bits.resize(numDimension, vector<bitset<subs>>(numBits));

    //// 前缀和、后缀和数组, 不包括本身
    //_for(i, 0, numDimension) {
    //	_for(j, 1, numBucket) {
    //		fix[0][i][numBucket - 1 - j] = fix[0][i][numBucket - j] + data[0][i][numBucket - j].size();
    //		fix[1][i][j] = fix[1][i][j - 1] + data[1][i][j - 1].size();
    //	}
    //	// 整个数组的和存在最后一个元素上
    //	fix[0][i][numBucket] = fix[0][i][0] + data[0][i][0].size();
    //	fix[1][i][numBucket] = fix[1][i][numBucket - 1] + data[1][i][numBucket - 1].size(); // Bug: 少了-1!!!
    //}

    // 前缀和数组(不包括本身)、后缀和数组(包括本身)
    _for(i, 0, numDimension)
    {
        fix[0][i][numBucket] = 0; // 后缀和
        fix[0][i][numBucket - 1] = data[0][i][numBucket - 1].size();
        fix[1][i][0] = 0;
        _for(j, 1, numBucket)
        {
            fix[0][i][numBucket - j - 1] = fix[0][i][numBucket - j] + data[0][i][numBucket - j - 1].size();
            fix[1][i][j] = fix[1][i][j - 1] + data[1][i][j - 1].size();
        }
        // 整个数组的和存在最后一个元素上
        // fix[0][i][numBucket] = fix[0][i][0];  // Bug: 导致后面刚开始算映射关系时fix[0][i][lowBktId]不合理
        fix[1][i][numBucket] = fix[1][i][numBucket - 1] + data[1][i][numBucket - 1].size(); // Bug: 少了-1!!!
    }

    // 当前应该映射到的bitId, 桶id, 下一个临界负载点
    int32_t lowBid, lowBktId, lowSubWorkLoad;
    // int32_t highBid, highBktId, highSubWorkLoad;
    int32_t subWorkLoadStep; // 每个维度上的subWorkLoadStep都不同, 但同一个维度上的low/high subWorkLoadStep是一样的
    _for(i, 0, numDimension)
    {
        // 基本不会出现
        if (fix[0][i][0] == 0) // 空维度
        {
            _for(j, 0, numBucket)
            {
                bitsID[i][j] = 0;       // 此时 bitset 里也是全 0
                endBucket[0][i][j] = j; // 遍历到等于endBucket[0][i][j]-1
                endBucket[1][i][j] = j; // 遍历到大于等于endBucket[1][i][j]
            }
            continue;
        }

        subWorkLoadStep = (fix[0][i][0] + numBits - 2) / (numBits - 1); // fix[1][i][numBucket]; numBits 算上了空桶，Eg. numBits=9, 则应分成 8 组

        // 由于是low/high都是动态的, 基本不可能共用同一套partition/cell,
        // 但这里low还是从左边开始数一个subWorkLoadStep的量, 保持一致
        // 或者(但是要改成)从右边数 剩余负载量 开始累加subWorkLoadStep（下面使用的这种）, 否则不清楚endBucket!
        // 0号low桶一定可以用到以 (numBits - 1) 为下标的bitset
        // 最后一个桶一定用不到 bitset
        // 举例: numBits=15(不是16), fix[0][i][0]=1000000, subWorkLoadStep=66667 (low上的后14个多1, high上的前14个多1)
        // fix[0][i][numBucket] / subWorkLoadStep=14, lowSubWorkLoad=66662
        // lowSubWorkLoad = fix[0][i][0] - (fix[0][i][0] - 1) / subWorkLoadStep * subWorkLoadStep; // 减一是为了避免正好整除
        // highSubWorkLoad = subWorkLoadStep;

        // 没有双重反向优化，不需要提前知道每过一个 subWorkLoadStep 的桶号位置了，Contain 数组的计算可以省去了
        // lowContain[i]=从右数(第一个覆盖)lowSubWorkLoad+(i-1)*subWorkLoadStep个订阅所到的桶号(i>0时)
        // vector<int> lowContain(numBits, numBucket); // Eg. numBits=9, lowContain[1~8] 有效
        // // highContain[i]=左数 i*subWorkLoadStep 个订阅所到的桶号
        // vector<int> highContain(numBits, 0);
        // int li = 1, hi = 1; // lowContain和highContain的下标
        // _for(j, 0, numBucket)
        // {
        //     if (fix[1][i][j] >= highSubWorkLoad)
        //     {                          // fix[1][i][numBucket]才包括全部, 最后不一定能进入if
        //         highContain[hi++] = j; // numBits=2时highContain[1]<=numBucket-1(右数第一个非空桶位置+1)
        //         highSubWorkLoad += subWorkLoadStep;
        //     }
        //     // 举例: fix[0][i][0]=1M, subWorkLoadStep=100000, numBits=10
        //     // li,lowSubWorkLoad = 1,100000; 2,200000; ... ; 9,900000; 10,1000000; 11,1100000
        //     if (fix[0][i][numBucket - j - 1] >= lowSubWorkLoad)
        //     {                                         // fix[0][i][0]就包括全部, 所以一定进入if
        //         lowContain[li++] = numBucket - j - 1; // numBits=2时lowContain[1]>=0 (左数第一个非空桶位置)
        //         lowSubWorkLoad += subWorkLoadStep;
        //     }
        // }
        // if (hi == numBits-1)             // Bug: 最后几个桶为空时hi会在for循环里增加到numBits; 最后一个桶非空时highContain[numBits]还没赋值
        //     highContain[hi] = numBucket; // numBits=2时highContain[1]=numBucket

        lowSubWorkLoad = fix[0][i][0] - (fix[0][i][0] - 1) / subWorkLoadStep * subWorkLoadStep; // 减一是为了避免正好整除
        // highSubWorkLoad = subWorkLoadStep;
        lowBid = 1;
        // highBid = numBits - 1; // high 上的动态数组没用上，这里只是计算一下，后面都是用的 low 上的动态分组方案, 由于 bitsID 少了一维，存不下，所以注释了
        lowBktId = numBucket;
        // highBktId = 0;
        vector<int32_t> loadBktPoint; // 从高到低记录 LVE 上的动态分组方案（每个分组的低边界桶）
        for (int lj = 0, rj = numBucket - 1; rj >= 0; lj++, rj--)
        {
            // HVE 上按动态分组方案的代码（改成共享 LVE 上的动态分组方案了）
            // 此时大于等于highSubWorkLoad了, 可以用bits, 因为bits覆盖到lj-1桶
            // if (fix[1][i][lj] >= highSubWorkLoad)
            // { // 第一个大于等于临界点的桶(j-1号, 前缀和不包含本身)作为bitset覆盖的终点桶
            //     highSubWorkLoad += subWorkLoadStep;
            //     highBid--;
            //     highBktId = lj; // 之后的桶只需遍历标记到大于等于 highBktId
            // }

            // Bug: 提前满了, 后面的所有桶都为空, 无需遍历了，直接用全量位集 0 号
            // if (fix[1][i][lj] == fix[1][i][numBucket])
            // {
            //     bitsID[i][lj] = 0;
            //     endBucket[1][i][lj] = lj; // bkt lj does not need to be marked
            // }
            // else
            // {
            //     bitsID[i][lj] = highBid;
            //     endBucket[1][i][lj] = highBktId; // 遍历到大于等于endBucket[1][i][j]
            // }

            // Bug: 提前满了, 前面一些序号小的桶都是空的, 单独考虑
            if (fix[0][i][rj] == fix[0][i][0] && data[0][i][rj].size() == 0) // 需要本桶也为空才行，否则全量位集不是全不匹配的，不能直接用
            {
                bitsID[i][rj] = 0;        // 全不匹配，直接用全量位集 0 号
                endBucket[0][i][rj] = rj; // 此时不需要遍历桶了
            }
            else
            {
                bitsID[i][rj] = lowBid;
                endBucket[0][i][rj] = lowBktId;
            }

            // 此时虽然大于等于lowSubWorkLoad了, 但仍不可以用bits, 因为bits要覆盖到rj号桶
            if (fix[0][i][rj] >= lowSubWorkLoad)
            {
                lowSubWorkLoad += subWorkLoadStep;
                lowBid++;
                lowBktId = rj;
                loadBktPoint.push_back(rj);
            }
        }
        if (loadBktPoint.size() + 1 != numBits)
        { // 记录了 numBits -1 个等分位桶号
            std:cerr << "loadBktPoint.size()= " << loadBktPoint.size() << " != numBits-1, numBits= " << numBits << std::endl;
        }
        // 根据 loadBktPoint 反推 HVE 上的 endBucket
        for (int32_t bj = 0, pj = loadBktPoint.size() - 1; bj < numBucket; bj++)
        {
            if (pj > 0 && bj == loadBktPoint[pj - 1]) // 进入下一个分组
                pj--;
            endBucket[1][i][bj] = loadBktPoint[pj];
        }
    } // each dimension

    int subID, b; // 起始标记数组的下标
    _for(i, 0, numDimension)
    { // 每个维度
        _for(j, 0, numBucket)
        {                     // 每个桶
            b = bitsID[i][j]; // 1 ~ numBits-1
            _for(k, 0, data[0][i][j].size())
            {
                subID = data[0][i][j][k].subID;
                bits[i][0][subID] = 1; // 0号bits每次必须标记
                _for(q, b + 1, numBits)
                    bits[i][q][subID] = 1;
            }
            _for(k, 0, data[1][i][j].size())
            {
                subID = data[1][i][j][k].subID;
                _for(q, 1, b) bits[i][q][subID] = 1;
            }
        }
    }
    // std::cout << "HEM5_ASO Stop.\n";
}

void HEM3_ASO::match_VASO(const Pub &pub, int &matchSubs)
{
    bitset<subs> gB; // register
    bitset<subs> bLocal;
    vector<bool> attExist(numDimension, false);
    vector<bool> attGroupExist(numAttrGroup, false);
    int value, att, buck;

    _for(i, 0, pub.size)
    {
        value = pub.pairs[i].value, att = pub.pairs[i].att, buck = value / buckStep;
        attExist[att] = true;
        attGroupExist[att / attrGroupSize] = true;

#ifdef DEBUG
        Timer markStart;
#endif // DEBUG
        _for(j, buck + 1, endBucket[0][att][buck]) for (auto &&iCob : data[0][att][j])
            gB[iCob.subID] = 1;
        _for(j, endBucket[1][att][buck], buck) for (auto &&iCob : data[1][att][j])
            gB[iCob.subID] = 1;
#ifdef DEBUG
        markTime += (double)markStart.elapsed_nano();
        Timer compareStart;
#endif // DEBUG
        for (auto &&iCob : data[0][att][buck])
            if (iCob.val > value)
                gB[iCob.subID] = 1;
        for (auto &&iCob : data[1][att][buck])
            if (iCob.val < value)
                gB[iCob.subID] = 1;
#ifdef DEBUG
        compareTime += (double)compareStart.elapsed_nano();
        Timer orStart;
#endif // DEBUG
#if BlockSize == 64
        gB = gB | bits[att][bitsID[att][buck]];
#else
        Util::bitsetOr(gB, bits[att][bitsID[att][buck]]);
#endif
#ifdef DEBUG
        orTime += (double)orStart.elapsed_nano();
#endif // DEBUG
    }

    // 处理空维度情况
#ifdef DEBUG
    Timer orStart;
#endif // DEBUG
    for (int AGi = 0, base; AGi < numAttrGroup; AGi++)
    {
        if (attGroupExist[AGi])
        {
            base = AGi * attrGroupSize;
            _for(att, base, min(base + attrGroupSize, numDimension)) // 遍历这个属性组的每个属性号
            {
                if (!attExist[att])
                {
#if BlockSize == 64
                    gB = gB | bits[att][0];
#else
                    Util::bitsetOr(gB, bits[att][0]);
#endif
                }
            }
        }
        else // 该属性组不存在谓词
        {
#if BlockSize == 64
            gB = gB | attrGroupBits[AGi];
#else
            Util::bitsetOr(gB, attrGroupBits[AGi]);
#endif
        }
    }
#ifdef DEBUG
    orTime += (double)orStart.elapsed_nano();
    Timer bitStart;
#endif // DEBUG
    //	_for(i, 0, subs) if (!b[i]) {
    //			++matchSubs;
    //			//cout << "HEM5_VAG matches sub: " << i << endl;
    //		}
    matchSubs = subs - gB.count();
#ifdef DEBUG
    bitTime += (double)bitStart.elapsed_nano();
#endif // DEBUG
}

void HEM3_ASO::match_RASO(const Pub &pub, int &matchSubs)
{
    bitset<subs> gB; // register
    bitset<subs> bLocal;
    vector<bool> attExist(numDimension, false);
    int value, att, buck;

    _for(i, 0, pub.size)
    {

        value = pub.pairs[i].value, att = pub.pairs[i].att, buck = value / buckStep;
        attExist[att] = true;

#ifdef DEBUG
        Timer markStart;
#endif // DEBUG
        _for(j, buck + 1, endBucket[0][att][buck]) for (auto &&iCob : data[0][att][j])
            gB[iCob.subID] = 1;
        _for(j, endBucket[1][att][buck], buck) for (auto &&iCob : data[1][att][j])
            gB[iCob.subID] = 1;
#ifdef DEBUG
        markTime += (double)markStart.elapsed_nano();
        Timer compareStart;
#endif // DEBUG
        for (auto &&iCob : data[0][att][buck])
            if (iCob.val > value)
                gB[iCob.subID] = 1;
        for (auto &&iCob : data[1][att][buck])
            if (iCob.val < value)
                gB[iCob.subID] = 1;
#ifdef DEBUG
        compareTime += (double)compareStart.elapsed_nano();
        Timer orStart;
#endif // DEBUG
#if BlockSize == 64
        gB = gB | bits[att][bitsID[att][buck]];
#else
        Util::bitsetOr(gB, bits[att][bitsID[att][buck]]);
#endif
#ifdef DEBUG
        orTime += (double)orStart.elapsed_nano();
#endif // DEBUG
    }

    // 处理空维度情况
#ifdef DEBUG
    Timer orStart;
#endif // DEBUG
    int att_group_no = att / attrGroupSize;
#if BlockSize == 64
    gB = gB | attrGroupBits[att_group_no];
#else
    Util::bitsetOr(gB, attrGroupBits[att_group_no]);
#endif
    int base = att_group_no * attrGroupSize;
    _for(att, base, min(base + attrGroupSize, numDimension))
    {
        if (!attExist[att])
        {
#if BlockSize == 64
            gB = gB | bits[att][0];
#else
            Util::bitsetOr(gB, bits[att][0]);
#endif
        }
    }
#ifdef DEBUG
    orTime += (double)orStart.elapsed_nano();
    Timer bitStart;
#endif
    //	_for(i, 0, subs) if (!b[i]) {
    //			++matchSubs;
    //			//cout << "HEM5_VAG matches sub: " << i << endl;
    //		}
    matchSubs = subs - gB.count();
#ifdef DEBUG
    bitTime += (double)bitStart.elapsed_nano();
#endif
}

void HEM3_ASO::match_RASO_parallel(const Pub &pub, int &matchSubs)
{
    vector<future<bitset<subs>>> thread_result;
    int seg = pub.size / parallelDegree;
    int remainder = pub.size % parallelDegree;
    int tId = 0, end;
    for (int begin = 0; begin < pub.size; begin = end, tId++)
    {
        if (tId < remainder)
            end = begin + seg + 1;
        else
            end = begin + seg;
        thread_result.emplace_back(threadPool.enqueue([this, &pub, begin, end]
                                                      {
            // 局部变量存栈里
			bitset<subs> gB; // register
			int value, att, buck;
			for (int i = begin; i < end; i++) {
				value = pub.pairs[i].value, att = pub.pairs[i].att, buck = value / buckStep;

                _for(j, buck + 1, endBucket[0][att][buck]) for (auto &&iCob: data[0][att][j])
                    gB[iCob.subID] = 1;
                _for(k, 0, data[0][att][buck].size()) if (data[0][att][buck][k].val > value)
                    gB[data[0][att][buck][k].subID] = 1;

                _for(j, endBucket[1][att][buck], buck) for (auto &&iCob : data[1][att][j])
                    gB[iCob.subID] = 1;
                _for(k, 0, data[1][att][buck].size()) if (data[1][att][buck][k].val < value)
                    gB[data[1][att][buck][k].subID] = 1;

#if BlockSize == 64
                gB = gB | bits[att][bitsID[att][buck]];
#else
                Util::bitsetOr(gB, bits[att][bitsID[att][buck]]);
#endif
            }
			return gB; }));
    }

    int att_group_no = pub.pairs[0].att / attrGroupSize;
    bitset<subs> gB = attrGroupBits[att_group_no];
    if (pub.size < attrGroupSize)
    {
        vector<bool> attExist(numDimension, false);
        for (auto &item : pub.pairs)
            attExist[item.att] = true;
        int base = att_group_no * attrGroupSize;
        _for(ai, base, min(base + attrGroupSize, numDimension))
        {
            if (!attExist[ai])
            {
#if BlockSize == 64
                gB = gB | bits[ai][0];
#else
                Util::bitsetOr(gB, bits[ai][0]);
#endif
            }
        }
    }
#ifdef DEBUG
    Timer mergeStart;
#endif
    for (int i = 0; i < thread_result.size(); i++)
    {
#if BlockSize == 64
        gB = gB | thread_result[i].get();
#else
        Util::bitsetOr(gB, thread_result[i].get());
#endif
    }
#ifdef DEBUG
    mergeTime += (double)mergeStart.elapsed_nano();
    Timer bitStart;
#endif
    matchSubs = subs - gB.count();
#ifdef DEBUG
    bitTime += (double)bitStart.elapsed_nano();
#endif
}

int HEM3_ASO::calMemory()
{
    long long size = 0; // Byte
    size += sizeof(bits) + sizeof(data) + sizeof(data[0]) + sizeof(data[1]);
    // cout << sizeof(bits) << " " << sizeof(data) << " " << sizeof(data[0]) << " " << sizeof(data[1]) << "\n";
    _for(i, 0, numDimension)
    {
        // 若每个维度上bits数组个数一样就是 sizeof(bitset<subs>)*numDimension*numBits
        size += sizeof(bitset<subs>) * (bits[i].size());
        size += sizeof(bits[i]) + sizeof(data[0][i]) * 2;
        // cout << i << ": " << sizeof(bits[i]) << " " << sizeof(data[0][i]) << " ";
        _for(j, 0, numBucket)
        {
            // cout << "j= " << j << sizeof(data[0][i][j]) << " " << sizeof(data[1][i][j]) << " ";
            size += sizeof(data[0][i][j]) + sizeof(data[1][i][j]);
            size += sizeof(Combo) * (data[0][i][j].size() + data[1][i][j].size());
        }
        // cout << "\n";
    }

    // 两个fix
    // cout << "fix: " << sizeof(fix) << " " << sizeof(fix[0]) << " " << sizeof(fix[0][10]) << sizeof(fix[1][7][20]) << "\n";
    size += sizeof(fix) + sizeof(fix[0]) * 2;
    if (fix[0].size() > 0)
    {
        size += sizeof(fix[0][0]) * fix[0].size() * 2 + sizeof(int32_t) * fix[0].size() * fix[0][0].size() * 2;
    }

    // attrGroupBits
    size += sizeof(bitset<subs>) * attrGroupBits.size();

    // 两个endBucket、一个bitsID
    size += sizeof(endBucket) * 3 + sizeof(endBucket[0][0]) * numDimension * 3 + sizeof(int) * numDimension * numBucket * 3;

    // cout << "endBucket size: " << sizeof(endBucket) << " " << sizeof(endBucket[0]) << " " << sizeof(endBucket[0][0]) << " " << sizeof(endBucket[0][0][0]) << "\n";
    size = size / 1024 / 1024; // MB
    return (int)size;
}

void HEM3_ASO::printRelation(int dimension_i)
{
    cout << "\n\nHEM3_D_ASO Map\n";
    if (dimension_i == -1)
        _for(i, 0, numDimension)
        {
            cout << "\nDimension " << i << "    LowBucket Predicates: " << fix[0][i][0] << "   ----------------\n";
            _for(j, 0, numBucket)
            {
                cout << "lBkt" << j << ": bID=" << bitsID[i][j] << ", eBkt=" << endBucket[0][i][j] << "; ";
                if (j % 5 == 0 && j > 0)
                    cout << "\n";
            }
            cout << "\n\nDimension " << i << "    HighBucket Predicates: " << fix[1][i][numBucket]
                 << "   ----------------\n";
            _for(j, 0, numBucket)
            {
                cout << "hBkt" << j << ": bID=" << bitsID[i][j] << ", eBkt=" << endBucket[1][i][j] << "; ";
                if (j % 5 == 0 && j > 0)
                    cout << "\n";
            }
        }
    else
    {
        cout << "\nDimension: " << dimension_i << "    LowBucket Predicates: " << fix[0][dimension_i][0]
             << "   ----------------\n";
        _for(i, 0, numBucket)
        {
            cout << "lBkt" << i << ": bID=" << bitsID[dimension_i][i] << ", eBkt=" << endBucket[0][dimension_i][i] << "; ";
            if (i % 5 == 0 && i > 0)
                cout << "\n";
        }
        cout << "\n\nDimension: " << dimension_i << "    HighBucket Predicates: " << fix[1][dimension_i][numBucket]
             << "   ----------------\n";
        _for(i, 0, numBucket)
        {
            cout << "hBkt" << i << ": bID=" << bitsID[dimension_i][i] << ", eBkt=" << endBucket[1][dimension_i][i] << "; ";
            if (i % 5 == 0 && i > 0)
                cout << "\n";
        }
    }
    cout << "\n\n";
}

vector<int> HEM3_ASO::calMarkNumForBuckets()
{
    vector<int> numMarking(numBucket, 0);
    _for(i, 0, numDimension)
    {
        _for(j, 0, numBucket)
        {
            numMarking[j] += data[0][i][j].size() + data[1][i][j].size();
            
            _for(k, j + 1, endBucket[0][i][j]) numMarking[j] += data[0][i][k].size();

            _for(k, endBucket[1][i][j], j) numMarking[j] += data[1][i][k].size();
        }
    }
    return numMarking;
}