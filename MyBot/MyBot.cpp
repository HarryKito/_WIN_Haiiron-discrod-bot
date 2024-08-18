#include "MyBot.h"
#include <dpp/dpp.h>

// FIXME: 
//	봇 명령 입출력에서 한글이 지원되지 않음...
// UTF-8 문제일까?

/* 토큰을 반드시 key 파일에 저장 후 사용해주세요!
 * 토근을 발급받는 방법!
 * https://dpp.dev/creating-a-bot-application.html
 * 봇을 초대할 때는 반드시 '봇' 범위와
 * 범위를 지정하여 초대하십시오. Ex. 'applications.commands'
 */

int main()
{
	const std::string BOT_TOKEN = get_discord_token();

	/* 봇 클러스터 생성*/
	dpp::cluster bot(BOT_TOKEN);

	/* 표준출력에 간단한 로그 메시지 출력 */
	bot.on_log(dpp::utility::cout_logger());

	/* on_ready에 슬래시 명령어 등록 */
	bot.on_ready([&bot](const dpp::ready_t& event)
	{
		/* run_once로 랩하여 재연결 때마다 실행되지 않도록 함.*/
		if (dpp::run_once<struct register_bot_commands>())
		{
			std::vector<dpp::slashcommand> commands
			{
				{ "ping", "We play \"Ping pong!\" for a test!", bot.me.id }
			};
			bot.global_bulk_command_create(commands);
		}
	});

	/* D++ 기능, 코루틴에 가장 최근에 추가된 기능으로 슬래시 명령을 처리 */
	bot.on_slashcommand([](const dpp::slashcommand_t& event) -> dpp::task<void>
	{
			if (event.command.get_command_name() == "ping")
				co_await event.co_reply("!P!O!N!G!");
			co_return;
	});

	/* 봇 시작 */
	bot.start(dpp::st_wait);

	return 0;
}
