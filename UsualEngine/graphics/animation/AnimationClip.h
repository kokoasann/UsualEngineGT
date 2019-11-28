/*!
 *@brief	アニメーションクリップ。
 */

#pragma once
namespace UsualEngine
{
	//////////////////////////////////////////////////////////////////////
	// 各種構造体
	//////////////////////////////////////////////////////////////////////
	/*!
	*@brief	アニメーションクリップのヘッダー。
	*/
	struct AnimClipHeader {
		std::uint32_t numKey;				//!<キーフレームの数。
		std::uint32_t numAnimationEvent;	//!<アニメーションイベントの数。
	};
	/*!
	*@brief	アニメーションイベント
	*@アニメーションイベントは未対応。
	* やりたかったら自分で実装するように。
	*/
	struct AnimationEventData {
		float	invokeTime;					//!<アニメーションイベントが発生する時間(単位:秒)
		std::uint32_t eventNameLength;		//!<イベント名の長さ。
	};
	/*!
	*@brief	キーフレーム。
	*/
	struct Keyframe {
		std::uint32_t boneIndex;	//!<ボーンインデックス。
		float time;					//!<時間。
		CMatrix transform;			//!<トランスフォーム。
	};
	/*!
	*@brief	キーフレーム。
	*/
	struct KeyframeRow {
		std::uint32_t boneIndex;	//!<ボーンインデックス。
		float time;					//!<時間。
		CVector3 transform[4];		//!<トランスフォーム。
	};

	class AnimationEvent
	{
	public:
		AnimationEvent(){}
		~AnimationEvent(){}

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		const std::string& GetEventName()
		{
			return m_eventName;
		}
		/// <summary>
		/// 
		/// </summary>
		/// <param name="name"></param>
		void SetEventName(std::string& name)
		{
			m_eventName = name;
		}

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		float GetInvokeTime()
		{
			return m_invokeTime;
		}
		/// <summary>
		/// 
		/// </summary>
		/// <param name="time"></param>
		void SetInvokeTime(float time)
		{
			m_invokeTime = time;
		}

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		bool IsInvoked()
		{
			return m_isInvoked;
		}
		/// <summary>
		/// 
		/// </summary>
		/// <param name="b"></param>
		void SetIsInvoked(bool b)
		{
			m_isInvoked = b;
		}

	private:
		std::string m_eventName;
		float m_invokeTime = 0.f;
		bool m_isInvoked = false;
	};

	/*!
	*@brief	アニメーションクリップ。
	*/
	class AnimationClip {
	public:
		//タイプ量が長ったらしくて、うざいのでstd::vector<KeyFrame*>の別名定義。
		using keyFramePtrList = std::vector<Keyframe*>;
		/*!
		* @brief	コンストラクタ
		*/
		AnimationClip()
		{
		}
		/*!
		*@brief	デストラクタ。
		*/
		~AnimationClip();
		/*!
		*@brief	アニメーションクリップをロード。
		*@param[in]	filePath	ファイルパス。
		*/
		void Load(const wchar_t* filePath,const char* clipname=nullptr);

		/*!
		*@brief	ループする？
		*/
		bool IsLoop() const
		{
			return m_isLoop;
		}
		/*!
		*@brief	ループフラグを設定する。
		*/
		void SetLoopFlag(bool flag)
		{
			m_isLoop = flag;
		}
		/*!
		*@brief
		*/
		const std::vector<keyFramePtrList>& GetKeyFramePtrListArray() const
		{
			return m_keyFramePtrListArray;
		}
		const keyFramePtrList& GetTopBoneKeyFrameList() const
		{
			return *m_topBoneKeyFramList;
		}

		int GetNumAnimationEvent()
		{
			return m_numAnimationEvent;
		}
		AnimationEvent* GetAnimationEvent()
		{
			return m_animationEventList;
		}
		const std::string& GetClipName()
		{
			return m_clipName;
		}

		int GetFrameNum() const
		{
			return m_keyFramePtrListArray[0].size();
		}

	private:
		std::string m_clipName = std::string();									//
		bool m_isLoop = false;									//!<ループフラグ。
		std::vector<Keyframe*> m_keyframes;						//全てのキーフレーム。
		std::vector<keyFramePtrList> m_keyFramePtrListArray;	//ボーンごとのキーフレームのリストを管理するための配列。
																//例えば、m_keyFramePtrListArray[0]は0番目のボーンのキーフレームのリスト、
																//m_keyFramePtrListArray[1]は1番目のボーンのキーフレームのリストといった感じ。
		keyFramePtrList* m_topBoneKeyFramList = nullptr;

		int m_numAnimationEvent = 0;
		AnimationEvent* m_animationEventList = nullptr;
	};

}