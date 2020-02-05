BEGIN {
    FS = " "
}

$1 == "[__DST_ALLOC]" &&
$2 == "malloc" {
    curr["mem",$3] += roundup($4)
    if (max["mem",$3] < curr["mem",$3]) {
        max["mem",$3] = curr["mem",$3]
    }
    mem[$3]
}

$1 == "[__DST_ALLOC]" &&
$2 == "free" {
    curr["mem",$3] -= roundup($4)
}

$1 == "[__REC_RECUR]" &&
$2 == "call" {
    curr["rec",$3] += 1
    if (max["rec",$3] < curr["rec",$3]) {
        max["rec",$3] = curr["rec",$3]
    }
    rec[$3]
}

$1 == "[__REC_RECUR]" &&
$2 == "ret" {
    curr["rec",$3] -= 1
}

function roundup(val) {
    tmp = 1
    logtmp = 0
    while (tmp < val) {
        tmp *= 2
        logtmp += 1
    }
    return tmp
}

function log2(val) {
    tmp = 1
    logtmp = 0
    while (tmp < val) {
        tmp *= 2
        logtmp += 1
    }
    return logtmp
}

END {
    for (key in mem)
        print "mem", key, log2(max["mem",key])
    for (key in rec)
        print "rec", key, log2(max["rec",key])
}
