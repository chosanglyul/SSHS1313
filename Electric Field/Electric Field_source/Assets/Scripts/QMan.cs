using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class QMan : MonoBehaviour
{
    int x, y, q;
    public Text xt, yt, qt;
    public void click()
    {
        string s = this.gameObject.name;
        int idx = 0, x;
        if (Int32.TryParse(s, out x))
            idx = Int32.Parse(s);
        GameObject.Find("Manager").GetComponent<Manager>().del(idx);
        Destroy(this.gameObject);
    }
    public void ok()
    {
        string s = this.gameObject.name;
        int idx = 0, k;
        bool ca = true;
        if (Int32.TryParse(s, out k))
            idx = Int32.Parse(s);
        s = xt.text;
        if (Int32.TryParse(s, out k))
            x = Int32.Parse(s);
        else ca = false;
        s = yt.text;
        if (Int32.TryParse(s, out k))
            y = Int32.Parse(s);
        else ca = false;
        s = qt.text;
        if (Int32.TryParse(s, out k))
            q = Int32.Parse(s);
        else ca = false;
        if (ca) GameObject.Find("Manager").GetComponent<Manager>().ud(idx, x, y, q);
        else print("ERROR");
    }
}
