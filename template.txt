package ru.pochtabank.fakturadbo.api.model.faktura;

import android.os.Parcel;

import com.google.gson.JsonObject;

import java.util.List;

import ru.pochtabank.dbo.base.model.BaseModel;
import ru.pochtabank.utils.data.ParcelUtils;

public class ElfBankInfoModel extends BaseModel {

    public static final Creator<ElfBankInfoModel> CREATOR = new Creator<ElfBankInfoModel>() {
        @Override
        public ElfBankInfoModel createFromParcel(Parcel source) {
            return new ElfBankInfoModel(source);
        }

        @Override
        public ElfBankInfoModel[] newArray(int size) {
            return new ElfBankInfoModel[size];
        }
    };

    private String bic;
    private String city;
    private List<PhoneModel> contacts;
    private String corAccount;
    private String fullName;
    private Integer id;
    private String inn;
    private String kpp;
    private String legalAddress;
    private Boolean mainBranch;
    private String name;
    private String nameShort;
    private Integer network;
    private String ownEmail;
    private String pushState;
    private ElfBankInfoSettingsModel settings;
    private String[] supportEmails;


    public ElfBankInfoModel(JsonObject in) {
        this.bic = getStringField(in, "bic");
        this.city = getStringField(in, "city");
        this.contacts = getModelList(in, "contacts", PhoneModel.class);
        this.corAccount = getStringField(in, "corAccount");
        this.fullName = getStringField(in, "fullName");
        this.id = getIntField(in, "id");
        this.inn = getStringField(in, "inn");
        this.kpp = getStringField(in, "kpp");
        this.legalAddress = getStringField(in, "legalAddress");
        this.mainBranch = getBooleanField(in, "mainBranch");
        this.name = getStringField(in, "name");
        this.nameShort = getStringField(in, "nameShort");
        this.network = getIntField(in, "network");
        this.ownEmail = getStringField(in, "ownEmail");
        this.pushState = getStringField(in, "pushState");
        this.settings = getModel(in, "settings", ElfBankInfoSettingsModel.class);
        this.supportEmails = getStringArray(in, "supportEmails");
    }

    protected ElfBankInfoModel(Parcel in) {
        if(ParcelUtils.readBooleanFromParcel(in)) this.bic = in.readString();
        if(ParcelUtils.readBooleanFromParcel(in)) this.city = in.readString();
        if(ParcelUtils.readBooleanFromParcel(in)) this.contacts = in.createTypedArrayList(PhoneModel.CREATOR);
        if(ParcelUtils.readBooleanFromParcel(in)) this.corAccount = in.readString();
        if(ParcelUtils.readBooleanFromParcel(in)) this.fullName = in.readString();
        if(ParcelUtils.readBooleanFromParcel(in)) this.id = in.readInt();
        if(ParcelUtils.readBooleanFromParcel(in)) this.inn = in.readString();
        if(ParcelUtils.readBooleanFromParcel(in)) this.kpp = in.readString();
        if(ParcelUtils.readBooleanFromParcel(in)) this.legalAddress = in.readString();
        if(ParcelUtils.readBooleanFromParcel(in)) this.mainBranch = in.readByte() != 0;
        if(ParcelUtils.readBooleanFromParcel(in)) this.name = in.readString();
        if(ParcelUtils.readBooleanFromParcel(in)) this.nameShort = in.readString();
        if(ParcelUtils.readBooleanFromParcel(in)) this.network = in.readInt();
        if(ParcelUtils.readBooleanFromParcel(in)) this.ownEmail = in.readString();
        if(ParcelUtils.readBooleanFromParcel(in)) this.pushState = in.readString();
        if(ParcelUtils.readBooleanFromParcel(in)) this.settings = in.readParcelable(ElfBankInfoSettingsModel.class.getClassLoader());
        if(ParcelUtils.readBooleanFromParcel(in)) this.supportEmails = in.createStringArray();
    }

    @Override
    public void writeToParcel(Parcel dest, int flags) {
        if (ParcelUtils.writeBooleanToParcel(dest, bic)) dest.writeString(bic);
        if (ParcelUtils.writeBooleanToParcel(dest, city)) dest.writeString(city);
        if (ParcelUtils.writeBooleanToParcel(dest, contacts)) dest.writeTypedList(contacts);
        if (ParcelUtils.writeBooleanToParcel(dest, corAccount)) dest.writeString(corAccount);
        if (ParcelUtils.writeBooleanToParcel(dest, fullName)) dest.writeString(fullName);
        if (ParcelUtils.writeBooleanToParcel(dest, id)) dest.writeInt(id);
        if (ParcelUtils.writeBooleanToParcel(dest, inn)) dest.writeString(inn);
        if (ParcelUtils.writeBooleanToParcel(dest, kpp)) dest.writeString(kpp);
        if (ParcelUtils.writeBooleanToParcel(dest, legalAddress)) dest.writeString(legalAddress);
        if (ParcelUtils.writeBooleanToParcel(dest, mainBranch)) dest.writeByte(mainBranch ? (byte)1 : (byte)0);
        if (ParcelUtils.writeBooleanToParcel(dest, name)) dest.writeString(name);
        if (ParcelUtils.writeBooleanToParcel(dest, nameShort)) dest.writeString(nameShort);
        if (ParcelUtils.writeBooleanToParcel(dest, network)) dest.writeInt(network);
        if (ParcelUtils.writeBooleanToParcel(dest, ownEmail)) dest.writeString(ownEmail);
        if (ParcelUtils.writeBooleanToParcel(dest, pushState)) dest.writeString(pushState);
        if (ParcelUtils.writeBooleanToParcel(dest, settings)) dest.writeParcelable(settings, 0);
        if (ParcelUtils.writeBooleanToParcel(dest, supportEmails)) dest.writeStringArray(supportEmails);
    }

    @Override
    public int describeContents() {
        return 0;
    }

    public String getBic() {
        return bic;
    }

    public String getCity() {
        return city;
    }

    public List<PhoneModel> getContacts() {
        return contacts;
    }

    public String getCorAccount() {
        return corAccount;
    }

    public String getFullName() {
        return fullName;
    }

    public Integer getId() {
        return id;
    }

    public String getInn() {
        return inn;
    }

    public String getKpp() {
        return kpp;
    }

    public String getLegalAddress() {
        return legalAddress;
    }

    public Boolean isMainBranch() {
        return mainBranch;
    }

    public String getName() {
        return name;
    }

    public String getNameShort() {
        return nameShort;
    }

    public Integer getNetwork() {
        return network;
    }

    public String getOwnEmail() {
        return ownEmail;
    }

    public String getPushState() {
        return pushState;
    }

    public ElfBankInfoSettingsModel getSettings() {
        return settings;
    }

    public String[] getSupportEmails() {
        return supportEmails;
    }
}