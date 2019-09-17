using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class Btncontrol : MonoBehaviour
{
    public GameObject scroll;
    public Text btn;
    bool isShow = true;
    public void click()
    {
        isShow = !isShow;
        scroll.SetActive(isShow);
        if (isShow) btn.text = "Hide";
        else btn.text = "Show";
    }
}
