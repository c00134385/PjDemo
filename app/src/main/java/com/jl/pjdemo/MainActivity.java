package com.jl.pjdemo;

import androidx.appcompat.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import butterknife.BindView;
import butterknife.ButterKnife;
import butterknife.OnClick;
import timber.log.Timber;

public class MainActivity extends AppCompatActivity {

    @BindView(R.id.btn_init)
    Button btnInit;

    @BindView(R.id.btn_uninit)
    Button btnUninit;

    @BindView(R.id.btn_start)
    Button btnStart;

    @BindView(R.id.btn_stop)
    Button btnStop;

    @BindView(R.id.btn_version)
    Button btnVersion;

    @BindView(R.id.tv_version)
    TextView tvVersion;

    @BindView(R.id.tv_result)
    TextView tvResult;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        ButterKnife.bind(this);
    }

    @OnClick({R.id.btn_init, R.id.btn_start, R.id.btn_stop, R.id.btn_version, R.id.btn_uninit})
    void onClick(View view) {
        Timber.d("id:%d", view.getId());
        switch (view.getId()) {
            case R.id.btn_version:
                tvVersion.setText(PjJni.getInstance().version());
                break;
            case R.id.btn_init:
                tvResult.setText(String.valueOf(PjJni.getInstance().init(new PjCb() {
                    @Override
                    public int onEvent(int event, String param) {
                        Timber.d("event:%d, param:%s", event, param);
                        return 0;
                    }
                })));
                break;
            case R.id.btn_start:
                tvResult.setText(String.valueOf(PjJni.getInstance().start("", 0)));
                break;
            case R.id.btn_stop:
                tvResult.setText(String.valueOf(PjJni.getInstance().stop()));
                break;
            case R.id.btn_uninit:
                tvResult.setText(String.valueOf(PjJni.getInstance().uninit()));
                break;
        }
    }
}
